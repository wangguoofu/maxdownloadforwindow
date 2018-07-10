#include "max_download.h"
#include "BootloaderScp.h"
#include "serial.h"
#include "max_debug.h"
#include <memory.h>
#include <QString>
/*
 BootloaderScp extends `Serial` by adding functions to read SCP commands.
*/

/*
 :Parameter port: serial port to use (/dev/tty* or COM*)
*/
int bootloaderscp__init__(QString portName)
{
  return serial_open(portName);
}

/*
*/
void bootloaderscp_setTimeOut(unsigned int val)
{
     serial_settimeout(val);
}

/*
  serial write
*/
int bootloaderscp_write(unsigned char *data, int datalen)
{
  return serial_write(data, datalen);
}

/*
  serial read
*/
int bootloaderscp_read(unsigned char*data, int datalen)
{
  return serial_read(data, datalen);
}
/*
Read a full packet
*/
scpcmd_data *bootloaderscp_readPacket(int quiet,int ignorlengh)
{
  static scpcmd_data scpcmd_v;
  scpcmd_data *scp_cmd = NULL;
  scpcmdhdr_data *hdr = NULL;
  int len = 0;

  scp_cmd = &scpcmd_v;
  memset(scp_cmd, 0x00, sizeof(scpcmd_data));

  hdr = bootloaderscp_readHeader(quiet);
  if(NULL == hdr)
  {
    MAXD_PRINT("error: no header\n");
    return NULL;
  }

  scpcmd_setHdr(scp_cmd, hdr);
  scp_cmd->len = 8;

  if(0 != hdr->dl)
  {
    len = serial_read(scp_cmd->data, hdr->dl);
   
    if(hdr->dl != len&&!ignorlengh)
    {
      MAXD_PRINT("error: expected hdr size != real one\n"); 
      MAXD_PRINT("real size = %d\n", len);
      MAXD_PRINT("expected size = %d\n", hdr->dl);
      return NULL;
    }

    scp_cmd->len += hdr->dl;
    scp_cmd->data_bin = scp_cmd->data;
    scp_cmd->data_len = hdr->dl;

    len = serial_read(scp_cmd->chk, 4);
    
    if(4 != len&&!ignorlengh)
    {
      MAXD_PRINT("error: expected hdr size != real one\n"); 
      MAXD_PRINT("real size = %d\n", len);
      MAXD_PRINT("expected size = %d\n", 4);
      return NULL;
    }
    scp_cmd->len += 4;
  }
  return scp_cmd;
}

/*
Read the packet header
*/
scpcmdhdr_data *bootloaderscp_readHeader(int quiet)
{
  static scpcmdhdr_data scpcmdhdr_v;
  static unsigned char data[8];
  scpcmdhdr_data *scp_hdr = NULL;
  int len = 0;
  
  scp_hdr = &scpcmdhdr_v;

  memset(scp_hdr, 0x00, sizeof(scpcmdhdr_data));
  len = serial_read(data,8);

  if(-1 == len)
  {
    if(!quiet)
    	MAXD_PRINT("error: timeout, no packet received\n");
    return NULL;
  }

  if(8 != len)
  {
    if(!quiet)
    {
    	MAXD_PRINT("error: expected hdr size != real one\n"); 
    	MAXD_PRINT("real size = %d\n", len);
    	MAXD_PRINT("expected size = %d\n", 8);
    }
    return NULL;
  }

  scpcmdhdr_parseData(scp_hdr, data, 8);

  return scp_hdr;
}

void bootloaderscp__finalization__(void)
{
  serial_close();
}
