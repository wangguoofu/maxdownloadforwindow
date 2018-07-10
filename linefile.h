#ifndef __LINEFILE_H__
#define __LINEFILE_H__

#include <stdio.h>
#include <string.h>

#ifdef MAX_DOWNLOAD_RAMBIN
struct max_linefile
{
  int stream;
  struct binautheninfo *info_p;
};
#define LINEFILE struct max_linefile
#elif defined(MAX_DOWNLOAD_1BIN)
struct max_linefile
{
  FILE *stream;
  struct binautheninfo *info_p;
};
#define LINEFILE struct max_linefile
#else
#define LINEFILE FILE
#endif

LINEFILE * linefile_open(char *filename);
int linefile_get(char *string, int n, LINEFILE* stream);
int linefile_seekbegin(LINEFILE *stream);
int linefile_close(LINEFILE *stream);

#endif
