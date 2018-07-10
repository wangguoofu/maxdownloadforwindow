#ifndef __BUFFERFILE_H__
#define __BUFFERFILE_H__

#if defined(MAX_DOWNLOAD_1BIN)||defined(MAX_DOWNLOAD_RAMBIN)
struct max_bufferfile
{
  int fd;
  struct binautheninfo *info_p;
};
#define MAX_MAX_FILE_COUNT 64
#endif

int bufferfile_open(char *name);
int bufferfile_write(int fd, unsigned char* data, int len);
int bufferfile_read(int fd, unsigned char* data, int len);
int bufferfile_close(int fd);

#endif
