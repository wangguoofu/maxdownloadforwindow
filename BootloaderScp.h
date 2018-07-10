#ifndef __BOOTLOADERSCP_H__
#define __BOOTLOADERSCP_H__

#include "ScpCmdHdr.h"
#include "ScpCmd.h"
#include <QString>

int bootloaderscp__init__(QString portName);
void bootloaderscp_setTimeOut(unsigned int val);
int bootloaderscp_write(unsigned char *data, int datalen);
int bootloaderscp_read(unsigned char*data, int datalen);
scpcmd_data *bootloaderscp_readPacket(int quiet,int ignorlengh);
scpcmdhdr_data *bootloaderscp_readHeader(int quiet);
void bootloaderscp__finalization__(void);

#endif
