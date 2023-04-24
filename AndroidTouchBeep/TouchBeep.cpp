#include <unistd.h>
#include <thread>
#include <mutex>
#include <string>
#include <errno.h>
#include "TouchBeep.h"

#ifdef LOG_MODULE_TAG
#undef LOG_MODULE_TAG
#endif

#define LOG_MODULE_TAG "TouchBeep"

using android::BBinder;
using android::legacy2aidl_pid_t_int32_t;
using android::legacy2aidl_uid_t_int32_t;
using android::NO_ERROR;
using android::String16;
using std::string;

sp<TouchBeep> TouchBeep::mToutchBeepIns = nullptr;

sp<TouchBeep> TouchBeep::getInstance(void)
{
    if (mToutchBeepIns == nullptr)
    {
        mToutchBeepIns = new TouchBeep();
        if (mToutchBeepIns == nullptr)
        {
            ALOGE("TouchBeep getInstance failed");
            return nullptr;
        }
    }
    return mToutchBeepIns;
}

TouchBeep::TouchBeep()
{
    mBitWidth = 0;
    mFrameSize = 0;
    mStreamType = AUDIO_STREAM_SYSTEM;
    mAudioTrack = nullptr;
    mChannelNum = 0;
    mSamplingRate = 0;
    mWrittingData = false;
}

TouchBeep::~TouchBeep()
{
    stopDataFlow();
    if (mAudioTrack != nullptr)
    {
        mAudioTrack->stop();
        mAudioTrack = nullptr;
    }
    if (mTouchBeepRes != nullptr)
    {
        fclose(mTouchBeepRes);
        mTouchBeepRes = nullptr;
    }
}

/**
 * @brief this is for init
 *
 */
bool TouchBeep::init(void)
{
    if (!prepareSoundTrack())
    {
        ALOGE("prepareSoundTrack failed");
        return false;
    }
    return true;
}

bool TouchBeep::loadSoundResource(void)
{
    string TouchBeepWav(ANDROID_TOUCH_BEEP_RESOURCE_NAME);
    if (access(TouchBeepWav.c_str(), F_OK | R_OK) != 0)
    {
        ALOGE("couldn't access TouchBeepSource[%s],error:%s", TouchBeepWav.c_str(), strerror(errno));
        return false;
    }
    mTouchBeepRes = fopen(TouchBeepWav.c_str(), "r");
    if (mTouchBeepRes == nullptr)
    {
        ALOGE("cloudn't fopen TouchBeepSource[%s],error:%s", TouchBeepWav.c_str(), strerror(errno));
        return false;
    }
    char index[4] = { 0 };
    /*Header部分*/
    // 4字节序,ChunkId，其值总为RIFF,fread后会自动偏移
    fread(index, sizeof(char), 4, mTouchBeepRes);
    if ((index[0] != 'R') || (index[1] != 'I') || (index[2] != 'F') || (index[3] != 'F'))
    {
        ALOGE("incorrect header,loadSoundResource failed");
        return false;
    }
    // 4字节ChunkSize
    fseek(mTouchBeepRes, 4, SEEK_CUR);
    // 4字节Format，其值总为WAVE
    fread(index, sizeof(char), 4, mTouchBeepRes);
    if ((index[0] != 'W') || (index[1] != 'A') || (index[2] != 'V') || (index[3] != 'E'))
    {
        ALOGE("incorrect format,loadSoundResource failed");
        return false;
    }
    /*Format Chunk部分*/
    // 4字节Subchunk1ID
    fread(index, sizeof(char), 4, mTouchBeepRes);
    if ((index[0] != 'f') || (index[1] != 'm') || (index[2] != 't') || (index[3] != ' '))
    {
        ALOGE("incorrect format,loadSoundResource failed");
        return false;
    }
    // 4字节Subchunk1Size，跳过
    fseek(mTouchBeepRes, 4, SEEK_CUR);
    // 2字节AudioFormat
    fread(index, sizeof(char), 2, mTouchBeepRes);
    if ((index[0] != 1) || (index[1] != 0))
    {
        ALOGE("incorrect AudioFormat,loadSoundResource failed");
        return false;
    }
    // 2字节Channel Number
    fread(index, sizeof(char), 2, mTouchBeepRes);
    mChannelNum = index[0];
    // 4字节采样率
    fread(index, sizeof(char), 4, mTouchBeepRes);
    mSamplingRate = (static_cast<uint32_t>(index[0]) & 0xFF) + ((static_cast<uint32_t>(index[1]) & 0xFF) << 8U) + ((static_cast<uint32_t>(index[2]) & 0xFF) << 16U) + ((static_cast<uint32_t>(index[3] & 0xFF) << 24U));
    // 4字节ByteRate，跳过
    fseek(mTouchBeepRes, 4, SEEK_CUR);
    // 2字节BlockAlign
    fread(index, sizeof(char), 2, mTouchBeepRes);
    mFrameSize = index[0];
    mBitWidth = (index[0] / mChannelNum) << 3U;
    return true;
}

bool TouchBeep::prepareSoundTrack(void)
{
    ALOGI("%s Entered", __FUNCTION__);
    if (!loadSoundResource())
    {
        ALOGE("loadSoundResource failed,plz check");
        return false;
    }
    android::content::AttributionSourceState attributionSource;
    String16 opPackageName("AndroidTouchBeepNative");
    attributionSource.packageName =
                 VALUE_OR_FATAL(legacy2aidl_String16_string(opPackageName));
    attributionSource.uid = VALUE_OR_FATAL(legacy2aidl_uid_t_int32_t(getuid()));
    attributionSource.pid = VALUE_OR_FATAL(legacy2aidl_pid_t_int32_t(getpid()));
    attributionSource.token = sp<BBinder>::make();
    mAudioTrack = new AudioTrack(attributionSource);
    if (mAudioTrack == nullptr)
    {
        ALOGE("new AudioTrack failed, prepareSoundTrack failed");
        return false;
    }
    mAudioTrack->set(mStreamType, mSamplingRate, getResBitWidthType(), mChannelNum, 0, AUDIO_OUTPUT_FLAG_NONE);
    if (mAudioTrack->initCheck() != NO_ERROR)
    {
        ALOGE("AudioTrack initCheck failed");
        return false;
    }
    mAudioTrack->setVolume(1.0, 1.0);
    ALOGI("%s Exited", __FUNCTION__);
    return true;
}

audio_format_t TouchBeep::getResBitWidthType()
{
    switch (mBitWidth)
    {
    case 8:
        return AUDIO_FORMAT_PCM_8_BIT;
    case 16:
        return AUDIO_FORMAT_PCM_16_BIT;
    case 24:
        return AUDIO_FORMAT_PCM_24_BIT_PACKED;
    case 32:
        return AUDIO_FORMAT_PCM_32_BIT;
    default:
        return AUDIO_FORMAT_INVALID;
    }
}

void TouchBeep::sendDataNonBlocking(void)
{
    std::thread writeData = std::thread(&TouchBeep::sendDataFlow, this);
    writeData.detach();
}

void TouchBeep::sendDataFlow(void)
{
    ALOGI("%s Entered", __FUNCTION__);
    if (mAudioTrack == nullptr)
    {
        ALOGE("mAudioTrack is null");
        return;
    }
    // NOTE:here we will start send data
    mAudioTrack->start();
    if (mTouchBeepRes != nullptr)
    {
        int16_t readBuf[4];
        size_t readLen = 0;
        std::lock_guard<std::mutex> g(mResourceMutex);
        mWrittingData.store(true);
        while (mWrittingData && (feof(mTouchBeepRes) == 0))
        {
            readLen = fread(readBuf, 1, mFrameSize, mTouchBeepRes);
            if (readLen != 0)
            {
                mAudioTrack->write(readBuf, readLen, true);
            }
            // ALOGI("read size is:%zu", readLen);
        }
        mAudioTrack->stop();
        // fclose(mTouchBeepRes);
        // mTouchBeepRes = nullptr;
        mWrittingData.store(false);
        fseek(mTouchBeepRes, 0, SEEK_SET);
    }
    ALOGI("sendDataFlow finished");
}
/**
 * @brief this is for stop
 *
 */
void TouchBeep::stopDataFlow(void)
{
    if (mWrittingData)
    {
        mWrittingData.store(false);
        // Wait for playing worker to stop
        std::lock_guard<std::mutex> g(mResourceMutex);
        if (nullptr != mTouchBeepRes)
        {
            fclose(mTouchBeepRes);
            mTouchBeepRes = nullptr;
        }
    }
}

/**
 * @brief this function will be used to paly sound
 *
 * @return true
 * @return false
 */
bool TouchBeep::playSoundEffect(void)
{
    ALOGI("%s Entered", __FUNCTION__);
    if ((mTouchBeepRes != nullptr) && (mAudioTrack != nullptr))
    {
        sendDataNonBlocking();
        return true;
    }
    return false;
}
