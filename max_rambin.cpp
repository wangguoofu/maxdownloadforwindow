#ifdef MAX_DOWNLOAD_RAMBIN
#include<string.h>
#include<stdio.h>
#include "max_download.h"
#include "max_debug.h"
#include "lineop.h"
#include "ramfile.h"

struct maxbin_info{
  struct binautheninfo* info_p;
  struct maxbin_info *next;
};

static struct maxbin_info *header_p = NULL;

int maxrambin_init(const unsigned char *data, unsigned int length)
{
  int stream = -1;
  char string[MAX_FILE_NAME_LEN];//file name maxlenth is 256
  char * ret = NULL;
  char * name = NULL;
  struct maxbin_info *pre = NULL, *next = NULL;
  struct binautheninfo *info_p = NULL;
  unsigned int offset = 0,dataoffset = 0;
  unsigned int len = 0;

  //save data info
  ramfile_init(data, length);

  stream = ramfile_open();
  if(stream<0)
  {
    MAXD_PRINT("error open ramfile\n");
    return -1;
  }
  
  ret = ramfile_gets(string, MAX_FILE_NAME_LEN, stream);
  MAXD_PRINT("len=%d,%s\n",strlen(string),string);
  if(!ret||strcmp(string,"MAXBIN!\n"))
  {
    MAXD_PRINT("not MAXBIN\n");
    return -1;
  }

  dataoffset+=strlen(string);
  
  
  ret = ramfile_gets(string, MAX_FILE_NAME_LEN, stream);
  while(ret&&strcmp(string, "endofheader\n"))
  {
    //MAXD_PRINT("file:%s\n",string);
    next = (struct maxbin_info *)malloc(sizeof(struct maxbin_info));
    if(!next) goto error_hd;
    memset(next, 0x00, sizeof(struct maxbin_info));

    name = (char *)malloc(strlen(string));
    if(!name)
      {
	free(next);
	goto error_hd;
      }

    info_p = (struct binautheninfo *)malloc(sizeof(struct binautheninfo));
    if(!info_p) 
      {
	free(next);
	free(name);
	goto error_hd;
      }
    memset(info_p, 0x00, sizeof(struct binautheninfo));

    dataoffset+=strlen(string);
    strcpy(name,line_strip(string));
    info_p->name = name;
    next->info_p = info_p;
    next->next = NULL;

    ret = ramfile_gets(string, MAX_FILE_NAME_LEN, stream);//get offset
    if(!ret) goto error_hd;
    dataoffset+=strlen(string);
    offset = atoi(line_strip(string));
    info_p->offset = offset;

    ret = ramfile_gets(string, MAX_FILE_NAME_LEN, stream);//get len
    if(!ret) goto error_hd;
    dataoffset+=strlen(string);
    len = atoi(line_strip(string));
    info_p->len = len;

    if(pre) pre->next = next;
    pre = next;
    if(!header_p) header_p = next;

    ret = ramfile_gets(string, MAX_FILE_NAME_LEN, stream);//get name
  }
  MAXD_PRINT("exit while\n");
  if(!ret) goto error_hd;
  dataoffset+=strlen(string);

  next = header_p;
  while(next)
  {
    next->info_p->offset+=dataoffset;
    next = next->next;
  }

  ramfile_close(stream);
  return 0;

 error_hd:
  ramfile_close(stream);
  maxrambin_free();
  return -1;
}

int maxrambin_free(void)
{
  struct maxbin_info *pre = NULL, *next = NULL;
  
  next = header_p;
  while(next)
  {
    pre = next;
    next = next->next;
    
    if(pre->info_p&&pre->info_p->name) free(pre->info_p->name);
    if(pre->info_p) free(pre->info_p);
    free(pre);
  }

  header_p = NULL;

  return 0;
}

struct binautheninfo *maxrambin_getinfo(char *name)
{
  struct maxbin_info *next = NULL;

  next = header_p;
  while(next)
  {
    if(!strcmp(next->info_p->name,name))
       return next->info_p;
    next = next->next;
  }
   
  return NULL;
}

#endif
