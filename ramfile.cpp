//ram file
#ifdef MAX_DOWNLOAD_RAMBIN
#include "max_download.h"
#include "ramfile.h"
#include <memory.h>

struct ramfile_info
{
  const unsigned char *data;
  unsigned int curoffset;
};

static const unsigned char* rambin = NULL;
static unsigned int binlen = 0;
static struct ramfile_info ramarray[MAX_MAX_RAMFILE_COUNT]={{NULL, 0},};//support max file count = MAX_MAX_FILE_COUNT

static int ramfile_getfd(void)
{
  int i;

  for(i=0; i<MAX_MAX_RAMFILE_COUNT; i++)
    {
      if(NULL == ramarray[i].data)
	return i;
    }

  return -1;
}

int ramfile_init(const unsigned char *data, unsigned int len)
{
  rambin = data;
  binlen =  len;
  
  return 0;
}

int ramfile_open()
{
  int fd = -1;

  fd = ramfile_getfd();
  if(fd == -1) return -1;

  ramarray[fd].data = rambin;
  ramarray[fd].curoffset = 0;
  return fd;
}



int ramfile_seek(int fd, int offset)
{
  if(offset>=binlen) return -1;

  ramarray[fd].data = rambin;
  ramarray[fd].curoffset = offset;

  return offset;
}

char* ramfile_gets(char *string, int n, int fd)
{
  char *p = NULL, *q = string;
  int count = 0;

  if(!ramarray[fd].data||ramarray[fd].curoffset>=binlen) return NULL;

  p = (char*)(ramarray[fd].data+ramarray[fd].curoffset);

  while(*p!='\n'&&count<n&&ramarray[fd].curoffset<binlen)
  {
    *string = *p;
    string++;
    p++;
    count ++;    
    ramarray[fd].curoffset++;
  }
  
  if(*p == '\n') 
    {
      *string++= '\n';
      ramarray[fd].curoffset++;
    }
  
  *string = '\0';

  return q;
}

int ramfile_read(int fd, unsigned char *data, int len)
{
  int left =0;
  
  if(!ramarray[fd].data||ramarray[fd].curoffset>=binlen) return 0;

  left = binlen-ramarray[fd].curoffset;

  if(left<len) len = left;

  memcpy(data, ramarray[fd].data+ramarray[fd].curoffset, len);

  ramarray[fd].curoffset+=len;

  return len;
}

int ramfile_close(int fd)
{
  if(ramarray[fd].data)
    {
      ramarray[fd].data = NULL;
      ramarray[fd].curoffset = -1;
    }
  return 0;
}
#endif
