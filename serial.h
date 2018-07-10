#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <QString>

void serial_settimeout(unsigned int val);
int serial_open(QString portName, bool wait=true);
int serial_write(unsigned char *data, int datalen);
int serial_read(unsigned char *data, int datalen);
void serial_close();

#endif
