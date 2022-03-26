#ifndef __LOG_UTILS_H__
#define __LOG_UTILS_H__

//for C++ call c functions
#ifdef __cplusplus
extern "C"
{
#endif

#include <utils/Log.h>

#define DEFAULT_LOG_LEVEL  10


#ifdef LOG_MODULE_TAG
#undef LOG_MODULE_TAG
#endif
#define LOG_MODULE_TAG "LogUtils"

//some default color here,which will show different color when log print in Linux

#define RTT_CTRL_RESET "[0m" // Reset to default colors
#define RTT_CTRL_CLEAR "[2J" // Clear screen, reposition cursor to top left
#define RTT_CTRL_TEXT_BLACK "[2;30m"
#define RTT_CTRL_TEXT_RED "[2;31m"
#define RTT_CTRL_TEXT_GREEN "[2;32m"
#define RTT_CTRL_TEXT_YELLOW "[2;33m"
#define RTT_CTRL_TEXT_BLUE "[2;34m"
#define RTT_CTRL_TEXT_MAGENTA "[2;35m"
#define RTT_CTRL_TEXT_CYAN "[2;36m"
#define RTT_CTRL_TEXT_WHITE "[2;37m"
#define RTT_CTRL_TEXT_BRIGHT_BLACK "[1;30m"
#define RTT_CTRL_TEXT_BRIGHT_RED "[1;31m"
#define RTT_CTRL_TEXT_BRIGHT_GREEN "[1;32m"
#define RTT_CTRL_TEXT_BRIGHT_YELLOW "[1;33m"
#define RTT_CTRL_TEXT_BRIGHT_BLUE "[1;34m"
#define RTT_CTRL_TEXT_BRIGHT_MAGENTA "[1;35m"
#define RTT_CTRL_TEXT_BRIGHT_CYAN "[1;36m"
#define RTT_CTRL_TEXT_BRIGHT_WHITE "[1;37m"
#define RTT_CTRL_BG_BLACK "[24;40m"
#define RTT_CTRL_BG_RED "[24;41m"
#define RTT_CTRL_BG_GREEN "[24;42m"
#define RTT_CTRL_BG_YELLOW "[24;43m"
#define RTT_CTRL_BG_BLUE "[24;44m"
#define RTT_CTRL_BG_MAGENTA "[24;45m"
#define RTT_CTRL_BG_CYAN "[24;46m"
#define RTT_CTRL_BG_WHITE "[24;47m"
#define RTT_CTRL_BG_BRIGHT_BLACK "[4;40m"
#define RTT_CTRL_BG_BRIGHT_RED "[4;41m"
#define RTT_CTRL_BG_BRIGHT_GREEN "[4;42m"
#define RTT_CTRL_BG_BRIGHT_YELLOW "[4;43m"
#define RTT_CTRL_BG_BRIGHT_BLUE "[4;44m"
#define RTT_CTRL_BG_BRIGHT_MAGENTA "[4;45m"
#define RTT_CTRL_BG_BRIGHT_CYAN "[4;46m"
#define RTT_CTRL_BG_BRIGHT_WHITE "[4;47m"

//here we changed standard android log to my own 

#ifdef ALOGD
#define LOGD(msg, ...) ALOGD("[D]%s::%s(%d): " msg, LOG_MODULE_TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif


#ifdef ALOGV
#define LOGV(msg, ...) ALOGV("[V]%s::%s(%d): " msg, LOG_MODULE_TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#ifdef ALOGW
#define LOGW(msg, ...) ALOGW("[W]%s::%s(%d): " msg, LOG_MODULE_TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#ifdef ALOGI
#define LOGI(msg, ...) ALOGI("[I]%s::%s(%d): " msg, LOG_MODULE_TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#ifdef ALOGD_IF
#define TRACE_D(level, msg, ...) ALOGD_IF((getTracerLevel() & level) == level, "[D]%s::%s(%d): " msg, LOG_MODULE_TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#ifdef ALOGI_IF
#define TRACE_I(level, msg, ...) ALOGI_IF((getTracerLevel() & level) == level, "[I]%s::%s(%d): " msg, LOG_MODULE_TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#ifdef ALOGE_IF
#define TRACE_E(level, msg, ...) ALOGE_IF((getTracerLevel() & level) == level, "[E]%s::%s(%d): " msg, LOG_MODULE_TAG, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif


//here we use more sepecific log level to contol logprint
//简单来说，我们自己的日志打印会借助安卓系统的等级与我们自己设置的等级来进行打印
#define LOGD(...) TRACE_I(0, ##__VA_ARGS__)
#define LOG1(...) TRACE_I(1, ##__VA_ARGS__)
#define LOG2(...) TRACE_I(2, ##__VA_ARGS__)
#define LOG4(...) TRACE_I(4, ##__VA_ARGS__)
#define LOG8(...) TRACE_I(8, ##__VA_ARGS__)
#define LOG16(...) TRACE_I(16, ##__VA_ARGS__)
#define LOG32(...) TRACE_I(32, ##__VA_ARGS__)
#define LOG64(...) TRACE_I(64, ##__VA_ARGS__)
#define LOG128(...) TRACE_I(128, ##__VA_ARGS__)

#define LOG_FUNCTION_ENTER LOGI("enter")
#define LOG_FUNCTION_EXIT LOGI("exit")

    int getLogLevel(void);
    void setLogLevel(int newLevel);
    void updateLogLevel(void);
	int logPrintForOutSider(int loglevel,char *format,...)

#ifdef __cplusplus
}
#endif

#endif
