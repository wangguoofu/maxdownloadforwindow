//for bin file
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> //文件控制定义
//#include <termios.h>//终端控制定义
#include <errno.h>
#include "max_debug.h"
#include "max_download.h"
#include "bufferfile.h"
#ifdef MAX_DOWNLOAD_RAMBIN
#include "ramfile.h"
#endif

#ifdef MAX_DOWNLOAD_RAMBIN
static struct max_bufferfile filearray[MAX_MAX_FILE_COUNT]={{-1,NULL},};//just support open 100 files at the same time
                                                                        // one filename must be opened only once at the same time

static int bufferfile_getfd(void)
{
  int i;

  for(i=0; i<MAX_MAX_FILE_COUNT; i++)
    {
      if(NULL == filearray[i].info_p)
	return i;
    }

  return -1;
}
int bufferfile_open(char *name)
{
  int fd=0;
  int index = 0;
  struct max_bufferfile *file_p = NULL;
  struct binautheninfo *info_p = NULL;

  fd=ramfile_open();
  if (fd < 0) {
    MAXD_PRINT("%s error open\n",name);
		return -1;
  }

  info_p = maxrambin_getinfo(name);
  if(!info_p)
    {
      MAXD_PRINT("%s error open\n",name);
      ramfile_close(fd);
      return -1;
    }
  
  index = bufferfile_getfd();
  if(index<0)
    {
      MAXD_PRINT("%s error open\n",name);
      ramfile_close(fd);
      return -1;
    }
  
  file_p = filearray+index;
  file_p->fd = fd;
  file_p->info_p = info_p;
  info_p->opoffset =0;

  ramfile_seek(fd, info_p->offset);

  return index;
}
/*
int bufferfile_write(int fd, unsigned char* data, int len)
{
  return write(fd, data, len);
}
*/
int bufferfile_read(int fd, unsigned char* data, int len)
{
  int ret = 0;
  int left = 0;

  if(filearray[fd].info_p->opoffset>=filearray[fd].info_p->len) return 0;
  
  left =filearray[fd].info_p->len- filearray[fd].info_p->opoffset; 
  if(left<len) len = left;

  ret = ramfile_read(filearray[fd].fd, data,len);
  filearray[fd].info_p->opoffset+=ret;
  return ret;
}

int bufferfile_close(int fd)
{
  if(filearray[fd].info_p)
  {
    ramfile_close(filearray[fd].fd);
    filearray[fd].fd = -1;
    filearray[fd].info_p->opoffset = 0;
    filearray[fd].info_p = NULL;
  }
  return 0;
}
#elif defined MAX_DOWNLOAD_1BIN
static struct max_bufferfile filearray[MAX_MAX_FILE_COUNT]={{-1,NULL},};//just support open 100 files at the same time
                                                                        // one filename must be opened only once at the same time

static int bufferfile_getfd(void)
{
  int i;

  for(i=0; i<MAX_MAX_FILE_COUNT; i++)
    {
      if(NULL == filearray[i].info_p)
	return i;
    }

  return -1;
}
int bufferfile_open(char *name)
{
  int fd=0;
  int index = 0;
  struct max_bufferfile *file_p = NULL;
  struct binautheninfo *info_p = NULL;

  fd=open(maxbin_getname(),O_RDWR);
  if (fd < 0) {
    MAXD_PRINT("%s error open\n",name);
		return -1;
  }

  info_p = maxbin_getinfo(name);
  if(!info_p)
    {
      MAXD_PRINT("%s error open\n",name);
      close(fd);
      return -1;
    }
  
  index = bufferfile_getfd();
  if(index<0)
    {
      MAXD_PRINT("%s error open\n",name);
      close(fd);
      return -1;
    }
  
  file_p = filearray+index;
  file_p->fd = fd;
  file_p->info_p = info_p;
  info_p->opoffset =0;

  lseek(fd, info_p->offset, SEEK_SET);

  return index;
}
/*
int bufferfile_write(int fd, unsigned char* data, int len)
{
  return write(fd, data, len);
}
*/
int bufferfile_read(int fd, unsigned char* data, int len)
{
  int ret = 0;
  int left = 0;

  if(filearray[fd].info_p->opoffset>=filearray[fd].info_p->len) return 0;
  
  left =filearray[fd].info_p->len- filearray[fd].info_p->opoffset; 
  if(left<len) len = left;

  ret = read(filearray[fd].fd, data,len);
  filearray[fd].info_p->opoffset+=ret;
  return ret;
}

int bufferfile_close(int fd)
{
  if(filearray[fd].info_p)
  {
    close(filearray[fd].fd);
    filearray[fd].fd = -1;
    filearray[fd].info_p->opoffset = 0;
    filearray[fd].info_p = NULL;
  }
  return 0;
}
#else
int bufferfile_open(char *name)
{
  int fd=0;

  fd=open(name,O_RDWR);
  if (fd < 0) {
    MAXD_PRINT("%s error open\n",name);
		return -1;
  }

  return fd;
}
/*
int bufferfile_write(int fd, unsigned char* data, int len)
{
  return write(fd, data, len);
}
*/
int bufferfile_read(int fd, unsigned char* data, int len)
{
  return read(fd, data,len);
}

int bufferfile_close(int fd)
{
  return close(fd);
}
#endif
