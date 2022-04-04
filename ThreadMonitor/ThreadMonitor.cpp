#include "ThreadMonitor.h"

#ifdef LOG_MODULE_TAG
#undef LOG_MODULE_TAG
#endif
#define LOG_MODULE_TAG "ThreadMonitor"

using namespace std;

namespace android
{
ThreadMonitor *ThreadMonitor::mThreadMonitor = nullptr;

ThreadMonitor::ThreadMonitor()
{
    ALOGI("%s", __FUNCTION__);
    mThreadMonitor = this;
    mActived = false;
    mThreadRunflag = false;
    mThreadMonitorPropertymap.clear();
}

ThreadMonitor::~ThreadMonitor()
{
    deinit();
}

ThreadMonitor *ThreadMonitor::getInstance()
{
    return mThreadMonitor;
}
void ThreadMonitor::init(void)
{
    ALOGI("%s", __FUNCTION__);
    mThreadRunflag = true;
    std::thread([this]() { monitorHandleThread(); }).detach();
}

void ThreadMonitor::deinit(void)
{
    ALOGI("%s", __FUNCTION__);
    mActived = false;
    mThreadRunflag = false;
    mThreadMonitorPropertymap.clear();
}

bool ThreadMonitor::registerMonitorService(const char *istring, int timeoutms)
{
    ALOGI("%s", __FUNCTION__);
    std::lock_guard<std::mutex> g(mThreadMonitorLock);
    uint32_t threadId = gettid();
    if (timeoutms <= 0)
    {
        ALOGE("Error:Invalid time parameter:[%d]ms for thread:[%d]", timeoutms, threadId);
        return false;
    }
    ALOGI("registerMonitorService->id:[%d],name[%s],timeoutms[%d]ms", threadId, istring, timeoutms);
    auto it = mThreadMonitorPropertymap.find(threadId);
    if (it != mThreadMonitorPropertymap.end())
    {
        ALOGI("thread[%d] already monitored,don't handle it", threadId);
        return true;
    }
    ThreadMonitor_T threadID_Property;
    stdtime threadregisterTime = std::chrono::steady_clock::now();
    threadID_Property.mThreadruntime = threadregisterTime;
    threadID_Property.mThreadtimeoutms = timeoutms;
    threadID_Property.mThreadBlockFlag = false;
    threadID_Property.mThreadname = istring;
    threadID_Property.mThreadBlockPoint = THREAD_BLOCK_POINT_DEF;
    mThreadMonitorPropertymap.insert(make_pair(threadId, threadID_Property));
    // mThreadMonitorPropertymap.insert(pair<uint32_t, ThreadMonitor_T>(threadId, threadID_Property));
    ALOGI("registerMonitorService->id:[%d],timeoutms[%d] success", threadId, timeoutms);
    return true;
}

bool ThreadMonitor::unregisterMonitorService()
{
    ALOGI("%s", __FUNCTION__);
    uint32_t threadId = gettid();
    ALOGI("unregisterMonitorService id->%d", threadId);
    std::lock_guard<std::mutex> g(mThreadMonitorLock);
    auto it = mThreadMonitorPropertymap.find(threadId);
    if (it == mThreadMonitorPropertymap.end())
    {
        ALOGI("thread[%d] not monitored,don't handle it", threadId);
        return true;
    }
    else
    {
        mThreadMonitorPropertymap.erase(threadId);
    }
    return true;
}

bool ThreadMonitor::updateThreadRuntime()
{
    std::lock_guard<std::mutex> g(mThreadMonitorLock);
    int threadId = gettid();
    stdtime threadCurTime = std::chrono::steady_clock::now();
    auto it = mThreadMonitorPropertymap.find(threadId);
    if (it == mThreadMonitorPropertymap.end())
    {
        ALOGI("thread[%d] not monitored,don't handle it", threadId);
        return true;
    }
    else
    {
        ALOGI("thread[%d][%s] last time excute cost->[%lld]ms", threadId, it->second.mThreadname.c_str(),
              duration_cast_mscount(threadCurTime - it->second.mThreadruntime));
        it->second.mThreadruntime = threadCurTime;
    }
    return true;
}

void ThreadMonitor::activate(void)
{
    mActived = true;
}
void ThreadMonitor::deactivate(void)
{
    mActived = false;
}

void ThreadMonitor::monitorHandleThread(void)
{
    static uint8_t count = 0;
    while (mThreadRunflag)
    {
        count++;
        if (count > 20)
        {
            count = 0;
            printMonitorInfo();
        }
        if (mActived)
        {
            std::lock_guard<std::mutex> g(mThreadMonitorLock);
            if (!mThreadMonitorPropertymap.empty())
            {
                auto curTime = std::chrono::steady_clock::now();
                for (auto &&it : mThreadMonitorPropertymap)
                {
                    auto delaytime = it.second.mThreadruntime + std::chrono::milliseconds(it.second.mThreadtimeoutms);
                    if (delaytime <= curTime)
                    {
                        it.second.mThreadBlockFlag = true;
                        ALOGI("Monitor Thread[%d][%s] blocked,blocked time:[%lld]ms,block point:[%s]", it.first,
                             it.second.mThreadname.c_str(), duration_cast_mscount(curTime - it.second.mThreadruntime),
                             it.second.mThreadBlockPoint.c_str());
                        if ((int)(duration_cast_mscount(curTime - it.second.mThreadruntime)) >= THREAD_BLOCK_TIME_MAX)
                        {
                            ALOGE("Monitor Thread[%d][%s] blocked up to %ds,evs exit", it.first,
                                 it.second.mThreadname.c_str(), THREAD_BLOCK_TIME_MAX / 1000);
                            abort();
                        }
                    }
                    else
                    {
                        it.second.mThreadBlockFlag = false;
                    }
                }
            }
            else
            {
                ALOGI("mThreadMonitorPropertymap has no member");
            }
        }
        else
        {
            ALOGD("Monitor thread not active,sleep");
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_MONITOR_CHECK_PERIODICTIME));
    }
    ALOGI("ThreadMonitor::monitorHandleThread Exited");
}

void ThreadMonitor::printMonitorInfo(void)
{
    // ALOGI("%s", __FUNCTION__);
    if (!mActived)
    {
        LOGD("monitorHandleThread is running,mActivated:[%s]", (mActived ? "true" : "false"));
        return;
    }
    std::lock_guard<std::mutex> g(mThreadMonitorLock);
    if (mThreadMonitorPropertymap.empty())
    {
        ALOGE("Error；No monitor member");
        return;
    }
    std::time_t curTime;
    curTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ALOGI("*************************");
    ALOGI("Current time:%s", ctime(&curTime));
    for (auto &&it : mThreadMonitorPropertymap)
    {
        ALOGI("Monitor thread id:%d,name:%s", it.first, it.second.mThreadname.c_str());
        // LOGD("Monitor thread run time:[%d]ms", duration_cast_mscount(it.second.mThreadruntime));
        ALOGI("Monitor thread max timeout:[%d]ms,is [%s]", it.second.mThreadtimeoutms,
              (it.second.mThreadBlockFlag ? "blocked" : "nonblocked"));
    }
    ALOGI("*************************");
}

void ThreadMonitor::recordThreadPoint(const string &pointInformation)
{
    std::lock_guard<std::mutex> g(mThreadMonitorLock);
    uint32_t threadId = gettid();
    auto it = mThreadMonitorPropertymap.find(threadId);
    if (it != mThreadMonitorPropertymap.end())
    {
        it->second.mThreadBlockPoint = pointInformation;
    }
}

} // namespace android
