//refer to BootloaderScp.py
//refer to class ScpCmdHdr()
#include "max_download.h"
#include "max_debug.h"
#include "ScpCmdHdr.h"
#include <memory.h>

/*
  :Parameter data: data for initialization
*/
void scpcmdhdr__init__(scpcmdhdr_data* scpcmdhdr_p,unsigned char *data,unsigned int length)
{
  //initialize scpcmdhdr_data
  scpcmdhdr_p->sync[0]=None;
  scpcmdhdr_p->sync[1]=None;
  scpcmdhdr_p->sync[2]=None;

  scpcmdhdr_p->ctl=None;
  scpcmdhdr_p->dl=None;
  scpcmdhdr_p->id=None;
  scpcmdhdr_p->cks=None;
  scpcmdhdr_p->extra=None;
  scpcmdhdr_p->data_bin=NULL;
  scpcmdhdr_p->data_length=0;


  if(data!=NULL&&length!=0)
    {
      scpcmdhdr_p->extra=scpcmdhdr_parseData(scpcmdhdr_p,data,length);
      MAXD_PRINT("scpcmdhdr_p.extra=%d\n",scpcmdhdr_p->extra);
      if(scpcmdhdr_p->extra!=None)
	MAXD_PRINT("waring: extra data in header\n");
    }
}

/*
  Fill the SCP command header with `data`

  :Parameter data: data to parse
*/
unsigned char scpcmdhdr_parseData(scpcmdhdr_data* scpcmdhdr_p,unsigned char *data,unsigned int length)
{
  char sync0,sync1,sync2;
  unsigned char extra=None;
  //int i;
  //for(i=0;i<length;i++)
  //  MAXD_PRINT("data[%d]=%x\n",i,data[i]);
  
  scpcmdhdr_p->data_bin=data;
  scpcmdhdr_p->data_length=length;

  if(length>=9)
    {
      sync0=data[0];
      sync1=data[1];
      sync2=data[2];

      scpcmdhdr_p->ctl=data[3];
      scpcmdhdr_p->dl=data[5]|data[4]<<0x08;
      scpcmdhdr_p->id=data[6];
      scpcmdhdr_p->cks=data[7];
      extra=data[8];

      scpcmdhdr_p->sync[0]=sync0;
      scpcmdhdr_p->sync[1]=sync1;
      scpcmdhdr_p->sync[2]=sync2;
    }
  else if(length==8)
    {
      sync0=data[0];
      sync1=data[1];
      sync2=data[2];

      scpcmdhdr_p->ctl=data[3];
      scpcmdhdr_p->dl=data[5]|data[4]<<0x08;
      scpcmdhdr_p->id=data[6];
      scpcmdhdr_p->cks=data[7];

      scpcmdhdr_p->sync[0]=sync0;
      scpcmdhdr_p->sync[1]=sync1;
      scpcmdhdr_p->sync[2]=sync2;
    }
  else
    MAXD_PRINT("%s:length error!!!!\n",__func__);

  return extra;
}
/*
"""
Implement == operator
"""
*/
int scpcmdhdr__eq__(scpcmdhdr_data* scpcmdhdr_p,scpcmdhdr_data* other)
{
  if(scpcmdhdr_p->data_length!=other->data_length)
    return 0;
  else
    return !memcmp(scpcmdhdr_p->data_bin,other->data_bin,scpcmdhdr_p->data_length);
}
/*
"""
Implement != operator
"""
*/
int scpcmdhdr__ne__(scpcmdhdr_data* scpcmdhdr_p,scpcmdhdr_data* other)
{
  if(scpcmdhdr_p->data_length!=other->data_length)
    return 1;
  else
    return memcmp(scpcmdhdr_p->data_bin,other->data_bin,scpcmdhdr_p->data_length);
}
/*
"""
Implement `str()` method
"""
*/
void scpcmdhdr__str__(scpcmdhdr_data *scpcmdhdr_p, char* str)
{
  sprintf(str,"%s","ScpCmdHdr:\n");
  sprintf(str+strlen(str),"- SYNC: %x,%x,%x\n",scpcmdhdr_p->sync[0],scpcmdhdr_p->sync[1],scpcmdhdr_p->sync[2]);
  sprintf(str+strlen(str),"- CTL:%d\n",scpcmdhdr_p->ctl);
  sprintf(str+strlen(str),"- DL:%d\n",scpcmdhdr_p->dl);
  sprintf(str+strlen(str),"- ID:%d\n",scpcmdhdr_p->id);
  sprintf(str+strlen(str),"- CKS:%d\n",scpcmdhdr_p->cks);
}

//00001110 10010000 11000011
//00110110 10010001 01100010 
