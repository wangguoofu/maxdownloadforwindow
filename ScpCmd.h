#ifndef __SCPCMD_H__
#define __SCPCMD_H__

#include "ScpCmdHdr.h"

typedef struct scpcmd_structure
{
  scpcmdhdr_data *hdr;
  unsigned char data[4*1024];//tmp 4k
  unsigned char chk[4];//?? not sure
  int len;

  unsigned char *data_bin;
  unsigned int data_len;
} scpcmd_data;

int scpcmd__init__(scpcmd_data *scpcmd_p, int buffer);
void scpcmd_setHdr(scpcmd_data *scpcmd_p, scpcmdhdr_data *scpcmdhdr_p);
int scpcmd_parseData(scpcmd_data *scmcmd_p, int buffer);
int scpcmd__eq__(scpcmd_data *scpcmd_p, scpcmd_data *other);
int scpcmd__ne__(scpcmd_data *scpcmd_p, scpcmd_data *other);
void scpcmd__str__(scpcmd_data *scpmd_p, char* str);

#endif
