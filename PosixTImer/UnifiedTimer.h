/***************************************************************** 
 *
 * FILE          : UnifiedTimer.cpp
 *
 * DESCRIPTION   : UnifiedTimer Class can be used when need a timer,the timer is created by Standard POSIX timer
 * VERSION       : 1.0.0
 * DATE          : 2019-12-17
 * AUTHOR        : FranzKafka
 * HISTORY       : 
 *
******************************************************************/
#ifndef __UNIFIED_TIMER_H__
#define __UNIFIED_TIMER_H__

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <utils/RefBase.h>

#include <iostream>

#define CLOCKID_REALTIME \
    CLOCK_REALTIME //This is a world-wide timer and the time can be changed by system,especially when NTP/GPS time
#define CLOCKID_CLOCK_MONOTONIC \
    CLOCK_MONOTONIC //This time can not be changed,measures time from some unspecified point in the past that does not

namespace android
{
//函数指针，返回值为void。想要声明一个指向改函数的指针，只需要用指针特换函数名即可
//bool (*p)(const string&, const string&);
using TimerCallBack = void (*)(sigval_t);

class Timer : public virtual android::RefBase
{
private:
    timer_t mTimerId;

public:
    Timer();
    ~Timer();
    /**
     * @brief 获取当前Timer状态
     *
     * @param No
     * 
     * @return 
     *   On success, true is returned.  On error, false is returned. 
     * 
     * @Owner     : FranzKafkaYu
     * @Date&Time : 2021-12-17
     */
    bool isTimerAlive(void);

    /**
     * @brief 使能定时器,到期执行回调
     *
     * @param [in]  callback        定时器回调函数
     * @param [in]  callbackParams  定时器回调函数参数
     * @param [in]  timeMs          定时器回调时间
     * @param [in]  periodic        周期回调标志位 true表示Timer到期后会周期回调 false只回调一次
	 * @param [in]  renable         是否可重复Renable，默认可以：这种需求一般对应于接收到某个任务后延迟固定时间的延时，重复接收重复延长
	                                如果Renable=false，该timer不可重复renable，必须当前timer到期后才可以执行
     * 
     * @return 
     *   On success, true is returned.  On error, false is returned. 
     * 
     * @Owner     : FranzKafkaYu
     * @Date&Time : 2021-12-17
	 */

    bool enable(TimerCallBack callback, sigval_t callbackParams, uint32_t timeMs, bool periodic, bool renable = true);

    /**
     * @brief 取消定时器
     *
     * @param No
     * 
     * @return 
     *   On success, true is returned.  On error, false is returned. 
     * 
     * @Owner     : FranzKafkaYu
     * @Date&Time : 2021-12-17
     */
    bool disable(void);
};
} // namespace android

#endif
