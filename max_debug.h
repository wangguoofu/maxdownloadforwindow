#ifndef __MAX_DEBUG_H__
#define __MAX_DEBUG_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //文件控制定义
//#include <termios.h>//终端控制定义
#include <errno.h>

void maxd_print(const char *fmt, ...);
char * maxd_strbuff(void);

#if 1
#define MAXD_PRINT(fmt, ...) \
  do { \
    maxd_print(fmt, ##__VA_ARGS__);                     \
   }while(0)
#else
#define MAXD_PRINT(fmt, ...) \
  do { \
    maxd_print("%-20s:%3d:: ",__FILE__,__LINE__);		\
    maxd_print(fmt, ##__VA_ARGS__);			\
   }while(0)
#endif
#endif
