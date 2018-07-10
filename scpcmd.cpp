//for scp cmd
//refer to BootloaderScp.py
#include "max_download.h"
#include "ScpCmd.h"
#include "bufferfile.h"
#include "max_debug.h"
#include <memory.h>

/*
  :Parameter buffer: a data buffer implementing the method `read(size)`
*/
int scpcmd__init__(scpcmd_data *scpcmd_p, int buffer)
{
  if(buffer>=0)
    return scpcmd_parseData(scpcmd_p, buffer);

  return -1;
}

/*
  Set the SCP command header
*/
void scpcmd_setHdr(scpcmd_data *scpcmd_p, scpcmdhdr_data *scpcmdhdr_p)
{
  scpcmd_p->hdr = scpcmdhdr_p;
}
/*
Fill the SCP command with data from `buffer`
:Parameter buffer: buffer implementing the method `read(size)`
*/
int scpcmd_parseData(scpcmd_data *scpcmd_p, int buffer)
{
  int len=0;
  static scpcmdhdr_data scpcmdhdr_v;
  scpcmdhdr_data * scpcmdhdr_p=NULL;
  //Read the header
  len = bufferfile_read(buffer, scpcmd_p->data, 8);
  if(len!=8)
  {
    MAXD_PRINT("error: expected hdr size != real one\n"); 
    MAXD_PRINT("real size = %d\n", len);
    MAXD_PRINT("expected size = 8\n");
    return -1;
  }

  scpcmdhdr_p = (scpcmdhdr_data *)&scpcmdhdr_v;//malloc(sizeof(scpcmdhdr_data));
  scpcmd_p->hdr = scpcmdhdr_p;
  scpcmd_p->len = 8;
  scpcmdhdr_parseData(scpcmdhdr_p, scpcmd_p->data, scpcmd_p->len);

  MAXD_PRINT("hdr.dl=%d\n",scpcmd_p->hdr->dl);

  if(scpcmd_p->hdr->dl!=0)
  {
    len = bufferfile_read(buffer, scpcmd_p->data, scpcmd_p->hdr->dl);
    if(len!=scpcmd_p->hdr->dl)
    {
      MAXD_PRINT("error: expected data size != real one\n"); 
      MAXD_PRINT("real size = %d\n", len);
      MAXD_PRINT("expected size = %d\n",scpcmd_p->hdr->dl);
      return -1;
    }

    scpcmd_p->data_bin = scpcmd_p->data;
    scpcmd_p->data_len = scpcmd_p->hdr->dl;

    len = bufferfile_read(buffer, scpcmd_p->chk, 4);
    if(len!=4)
    {
      MAXD_PRINT("error: expected chk size != real one\n"); 
      MAXD_PRINT("real size = %d\n", len);
      MAXD_PRINT("expected size = 4\n");
      return -1;
    }

    return 0;
  }
  else
  {
    //MAXD_PRINT("scpcmd_p->hdr.dl=0!!!!\n");//this is no error
    memset(scpcmd_p->data, 0x00, 4*1024);
    memset(scpcmd_p->chk, 0x00, 4);
    
    return 0;
  }
}

/*
Implement == operator
*/
int scpcmd__eq__(scpcmd_data *scpcmd_p, scpcmd_data *other)
{
  if(scpcmd_p->data_len != other->data_len)
    return 0;
  else
    return !memcmp(scpcmd_p->data_bin, other->data_bin, scpcmd_p->data_len)
      &&scpcmdhdr__eq__(scpcmd_p->hdr,other->hdr);
}

/*
Implement != operator
*/
int scpcmd__ne__(scpcmd_data *scpcmd_p, scpcmd_data *other)
{
  if(scpcmd_p->data_len != other->data_len)
    return 1;
  else
    return memcmp(scpcmd_p->data_bin, other->data_bin, scpcmd_p->data_len)
      &&scpcmdhdr__ne__(scpcmd_p->hdr,other->hdr);
}


/*
"""
Implement `str()` method
"""
*/
void scpcmd__str__(scpcmd_data *scpcmd_p, char* str)
{
  int i;
  sprintf(str,"%s","'ScpCmd:\n");
  scpcmdhdr__str__(scpcmd_p->hdr,str+strlen(str));

  sprintf(str+strlen(str),"Data:\n");
  for(i=0; i<scpcmd_p->data_len; i++)
    sprintf(str+strlen(str),"%02x",scpcmd_p->data[i]);
  sprintf(str+strlen(str),"\n");

  sprintf(str+strlen(str),"Checksum:");
  if(scpcmd_p->data_len)
  {
    for(i=0; i<4; i++)
      sprintf(str+strlen(str),"%02x",scpcmd_p->chk[i]);
  }
  sprintf(str+strlen(str),"\n");
}
