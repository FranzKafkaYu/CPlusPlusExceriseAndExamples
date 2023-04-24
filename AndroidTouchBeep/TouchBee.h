/**
 * @file TouchBeep.h
 * @brief
 * @author FranzKafka
 * @version 1.0
 * @date 
 *
 * @copyright Copyright (c) 2023
 *
 * @par 修改日志:this file is used for play TouchBeep
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td> <td>1.0     <td>wangh     <td>内容
 * </table>
 */

#ifndef ANDROID_TOUCH_BEEP_H
#define ANDROID_TOUCH_BEEP_H

#include <stdio.h>
#include <utils/Errors.h>
#include <media/AudioParameter.h>
#include <media/AudioTrack.h>
#include <media/AidlConversion.h>

#include <android/hidl/manager/1.0/IServiceManager.h>
#include <media/audiohal/DeviceHalInterface.h>
#include <utils/RefBase.h>

using ::android::AudioTrack;
using ::android::sp;

// 原始素材为ogg格式，需要转换为wav格式
#define ANDROID_TOUCH_BEEP_RESOURCE_NAME "/system/etc/sound/Effect_Tick.wav"

class TouchBeep : public virtual android::RefBase
{
public:
    TouchBeep();
    ~TouchBeep();
    // TouchBeep(const string resName);
    bool init(void);
    bool playSoundEffect(void);
    // Get Single
    static sp<TouchBeep> getInstance(void);

private:
    int mChannelNum;
    int mSamplingRate;
    int mFrameSize;
    int mBitWidth;
    std::mutex mResourceMutex;
    FILE *mTouchBeepRes;
    sp<AudioTrack> mAudioTrack;
    std::atomic<bool> mWrittingData;
    audio_stream_type_t mStreamType;
    static sp<TouchBeep> mToutchBeepIns;

private:
    bool loadSoundResource(void);
    bool prepareSoundTrack(void);
    audio_format_t getResBitWidthType(void);
    void sendDataNonBlocking(void);
    void sendDataFlow(void);
    void stopDataFlow(void);
};

#endif