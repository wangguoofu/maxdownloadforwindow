#ifndef __SERIAL_SENDER_H__
#define __SERIAL_SENDER_H__

#include "linefile.h"
#include <QString>

extern MasterThread *pMasterThread;

void serial_sender_stop(bool flag);
int serial_sender_parse_scpcmd_file(LINEFILE* cmds_file, int bl_emulation);
#ifdef MAX_DOWNLOAD_RAMBIN
int serial_sender_main(QString portName, const unsigned char *bin, unsigned int length, char *packet_list_name);
#else
int serial_sender_main(char *maxbin_name, char *packet_list_name);
#endif
#endif
