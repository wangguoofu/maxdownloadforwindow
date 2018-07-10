#ifndef __MAX_DOWNLOAD_H__
#define __MAX_DOWNLOAD_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //文件控制定义
//#include <termios.h>//终端控制定义
#include <errno.h>

#ifndef None
#define None 0 
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#define MAX_FILE_NAME_LEN 256

#if defined(MAX_DOWNLOAD_1BIN)||defined(MAX_DOWNLOAD_RAMBIN)
struct binautheninfo
{
  unsigned int offset;
  unsigned int len;
  unsigned int opoffset;
  char *name;
};
#endif

#ifdef MAX_DOWNLOAD_RAMBIN
int maxrambin_init(const unsigned char *data, unsigned int len);
int maxrambin_free(void);
struct binautheninfo *maxrambin_getinfo(char *name);
#elif defined(MAX_DOWNLOAD_1BIN)
int maxbin_init(char *filename);
int maxbin_free(void);
struct binautheninfo *maxbin_getinfo(char *name);
char *maxbin_getname(void);
#endif

#endif
