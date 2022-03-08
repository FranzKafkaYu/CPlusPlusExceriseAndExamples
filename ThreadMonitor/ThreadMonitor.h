#ifndef __THREAD_MONITOR_H__
#define __THREAD_MONITOR_H__

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <utils/RefBase.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <map>
#include <set>
#include <thread>

#define TIME_POINTE_INITIAL_VALUE 0
#define THREAD_MONITOR_CHECK_PERIODICTIME 50                                                      //单位：ms
#define THREAD_BLOCK_TIME_MAX 40000                                                               //单位：ms
#define duration_cast_mscount(x) std::chrono::duration_cast<std::chrono::milliseconds>(x).count() //单位转换,计算ms
#define duration_cast_scount(x) std::chrono::duration_cast<std::chrono::seconds>(x).count()       //单位转换,计算s
#define THREAD_BLOCK_POINT_DEF "normal"

using namespace std;
typedef std::thread::id tid_t;
typedef std::chrono::time_point<std::chrono::steady_clock> stdtime;

namespace android
{

typedef struct
{
    stdtime mThreadruntime;
    uint32_t mThreadtimeoutms;
    bool mThreadBlockFlag;
    string mThreadname;
    string mThreadBlockPoint;
} ThreadMonitor_T;
class ThreadMonitor : public virtual android::RefBase
{
private:
    bool mActived;
    bool mThreadRunflag;
    std::mutex mThreadMonitorLock;

public:
    std::map<uint32_t, ThreadMonitor_T> mThreadMonitorPropertymap;

public:
    static ThreadMonitor *mThreadMonitor;
    static ThreadMonitor *getInstance();
    ThreadMonitor();
    ~ThreadMonitor();
    void init(void);
    void deinit(void);
    void activate(void);
    void deactivate(void);
    bool registerMonitorService(const char *istring, int timeoutms);
    bool unregisterMonitorService();
    bool updateThreadRuntime();
    void recordThreadPoint(const string &pointInformation);

private:
    void monitorHandle(void);
    void monitorHandleThread(void);
    void printMonitorInfo(void);
};
} // namespace android

#endif
