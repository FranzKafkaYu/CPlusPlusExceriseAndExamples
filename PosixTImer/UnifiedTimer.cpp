/*****************************************************************
 *
 * FILE          : UnifiedTimer.cpp
 *
 * DESCRIPTION   : UnifiedTimer Class can be used when need a timer,the timer is
 *created by Standard POSIX timer VERSION       : 1.0.0 DATE          :
 *2019-12-17 AUTHOR        : FranzKafka HISTORY       :
 *
 ******************************************************************/
#include "UnifiedTimer.h"

#ifdef LOG_MODULE_TAG
#undef LOG_MODULE_TAG
#endif
#define LOG_MODULE_TAG "UnifiedTimer"

namespace android
{
Timer::Timer()
{
    mTimerId = NULL;
}

Timer::~Timer()
{
    disable();
}
bool Timer::isTimerAlive(void)
{
    if (mTimerId != NULL)
    {
        return true;
    }
    return false;
}
bool Timer::enable(TimerCallBack callback, sigval_t callbackParams, uint32_t timeMs, bool periodic, bool renable)
{
    if ((!renable) && (mTimerId != NULL))
    {
        return true;
    }
    ALOGI("Enable timer, time:[%d ms], periodic:[%s]\r\n", timeMs, periodic == true ? "true" : "false");
    struct sigevent evp;
    struct itimerspec it;
    struct timespec interval;
    disable();
    memset(&evp, 0, sizeof(struct sigevent));
    memset(&it, 0, sizeof(struct itimerspec));
    memset(&interval, 0, sizeof(struct timespec));
    evp.sigev_value = callbackParams;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = callback;
    //use timer_create to creat time,kernerl will allocate a timer
    if (timer_create(CLOCKID_CLOCK_MONOTONIC, &evp, &mTimerId) == -1)
    {
        ALOGE("Failed to timer_create, err:%s", strerror(errno));
        return false;
    }
    interval.tv_sec = timeMs / 1000;
    interval.tv_nsec = (timeMs % 1000) * 1000000;
    if (periodic)
    {
        it.it_interval = interval;
    }
    it.it_value = interval;
    //Arm (start) or disarm (stop) a timer
    if (timer_settime(mTimerId, 0, &it, NULL) == -1)
    {
        ALOGE("Failed to timer_settime, err:%s", strerror(errno));
        disable();
        return false;
    }
    ALOGI("Enable timer success, mTimerId:[%lu]", (unsigned long)mTimerId);
    return true;
}
bool Timer::disable(void)
{
    int ret = 0;
    if (mTimerId != NULL)
    {
        LOGE("Disable timer\r\n");
        //Disarm and delete a timer
        ret = timer_delete(mTimerId);
        if (ret == -1)
        {
            if (errno == EINVAL)
            {
                LOGE("This posix timer is invalid");
            }
            else
            {
                LOGE("Failed to timer_delete, err:%s", strerror(errno));
                return false;
            }
        }
        mTimerId = NULL;
        LOGI("Disable timer success\r\n");
    }
    return true;
}
} // namespace android