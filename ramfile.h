//ram file
#ifndef __RAMFILE_H__
#define __RAMFILE_H__

#define MAX_MAX_RAMFILE_COUNT 64

int ramfile_init(const unsigned char *data, unsigned int len);
int ramfile_open();
int ramfile_seek(int fd, int offset);
char* ramfile_gets(char *string, int n, int fd);
int ramfile_read(int fd, unsigned char *data, int len);
int ramfile_close(int fd);

#endif
