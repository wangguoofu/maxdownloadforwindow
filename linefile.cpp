//for cmds file
#include<string.h>
#include<stdio.h>
#include "linefile.h"
#include "max_debug.h"
#include "max_download.h"
#ifdef MAX_DOWNLOAD_RAMBIN
#include "ramfile.h"
#endif

#ifdef MAX_DOWNLOAD_RAMBIN
LINEFILE* linefile_open(char *filename)
{
  LINEFILE *linefile_p = NULL;
  struct binautheninfo* info_p = NULL;
  int stream = -1;

  stream = ramfile_open();
  if(stream<-1) 
    {
      MAXD_PRINT("error open ramfile\n");
      return NULL;
    }

  info_p = maxrambin_getinfo(filename);
  if(!info_p)
    {
      MAXD_PRINT("error open %s\n", filename);
      ramfile_close(stream);
      return NULL;
    }

  linefile_p = (LINEFILE *)malloc(sizeof(LINEFILE));
  if(!linefile_p) 
    {
      MAXD_PRINT("error open %s\n", filename);
      ramfile_close(stream);
      return NULL;
    }

  linefile_p->info_p = info_p;
  info_p->opoffset = 0;
  linefile_p->stream = stream;
  ramfile_seek(stream,info_p->offset);

  return linefile_p;
}

int linefile_get(char *string, int n, LINEFILE* stream)
{
  char * ret=NULL;
  
  if(stream->info_p->opoffset>=stream->info_p->len) return 0;

  ret = ramfile_gets(string, n, stream->stream);
  if(ret)
  {
    stream->info_p->opoffset+=strlen(string);
    return 1;
  }
  else
    return 0;
  
}

int linefile_seekbegin(LINEFILE *stream)
{
    ramfile_seek(stream->stream, stream->info_p->offset);
    stream->info_p->opoffset = 0;
    return 0;
}


int linefile_close(LINEFILE *stream)
{
  if(stream)
  {
    stream->info_p->opoffset = 0;
    ramfile_close(stream->stream);
    stream->stream = -1;
    free(stream);
    stream = NULL;
  }
  return 0;
}
#elif defined(MAX_DOWNLOAD_1BIN)
LINEFILE* linefile_open(char *filename)
{
  LINEFILE *linefile_p = NULL;
  struct binautheninfo* info_p = NULL;
  FILE* stream = NULL;

  stream = fopen(maxbin_getname(),"r");
  if(!stream) 
    {
      MAXD_PRINT("error open %s\n", filename);
      return NULL;
    } 

  info_p = maxbin_getinfo(filename);
  if(!info_p)
    {
      MAXD_PRINT("error open %s\n", filename);
      fclose(stream);
      return NULL;
    }

  linefile_p = malloc(sizeof(LINEFILE)); 
  if(!linefile_p) 
    {
      MAXD_PRINT("error open %s\n", filename);
      fclose(stream);
      return NULL;
    }

  linefile_p->info_p = info_p;
  info_p->opoffset = 0;
  linefile_p->stream = stream;
  fseek(stream,info_p->offset,SEEK_SET);

  return linefile_p;
}

int linefile_get(char *string, int n, LINEFILE* stream)
{
  char * ret=NULL;

  if(stream->info_p->opoffset>=stream->info_p->len) return 0;

  ret = fgets(string, n, stream->stream);
  if(ret)
  {
    stream->info_p->opoffset+=strlen(string);
    return 1;
  }
  else
    return 0;
  
}

int linefile_seekbegin(LINEFILE *stream)
{
    fseek(stream->stream, stream->info_p->offset,SEEK_SET);
    stream->info_p->opoffset = 0;
    return 0;
}

int linefile_close(LINEFILE *stream)
{
  if(stream)
  {
    stream->info_p->opoffset = 0;
    fclose(stream->stream);    
    stream->stream = NULL;
    free(stream);
    stream = NULL;
  }
  return 0;
}
#else
LINEFILE* linefile_open(char *filename)
{
  FILE* stream;
  stream = fopen(filename,"r");
  return stream;
}

int linefile_get(char *string, int n, LINEFILE* stream)
{
  char * ret=NULL;

  ret = fgets(string, n, stream);

  if(!ret)
    return 0;
  else
    return 1;
}

int linefile_seekbegin(LINEFILE *stream)
{
    return fseek(stream,0,SEEK_SET);
}

int linefile_close(LINEFILE *stream)
{
  return fclose(stream);
}
#endif
