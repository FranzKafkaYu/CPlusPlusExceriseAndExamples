#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define LOG_PRINT_MAX_BUFFER_LENGTH 2048
#define LOG_PRINT_LEVEL_INFO        0
#define LOG_PRINT_LEVEL_WARNING     2
#define LOG_PRINT_LEVEL_DEBUG       4
#define LOG_PRINT_LEVEL_VERBOSE     8
#define LOG_PRINT_LEVEL_ERROR       10

static int currentLoglevel = DEFAULT_LOG_LEVEL;

int getLogLevel(void)
{
    return currentLoglevel;
}

void updataLogLevel(void)
{
    setLogLevel();
    return;
}

void setLogLevel(int newLevel)
{
    if (newLevel == currentLoglevel)
    {
        return;
    }
    currentLoglevel = newLevel;
    ALOGI("set current log level:[%d]", currentLoglevel);
    return;
}

/*
 *This function will be used for third party
 *We can give its pointer to third party
 */
int logPrintForOutSider(int loglevel,char *format,...)
{
  if(loglevel>=currentLoglevel)
  {
    return -1;
  }
  char* logBuffer=NULL;
  logBuffer=(char*)calloc(1,LOG_PRINT_MAX_BUFFER_LENGTH);
  va_list args;
  va_start(args,format);
  //here we should notice that the vsprintf may fail
  vsprintf(logBuffer,format,args);
  va_end(args);
  //printf("%s\r\n",logBuffer);
  LOGI("[OutSider][%d]:%s",loglevel,logBuffer);
  free(logBuffer);
  logBuffer=NULL;
}
