//for uart operation
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "max_debug.h"
#include "max_delay.h"
#include "win_qextserialport.h"

extern bool stopSend;
static Win_QextSerialPort *my_serialport = NULL;
static unsigned int timeout_val = 1000;//us

void serial_settimeout(unsigned int val)
{
     timeout_val = val;
}

int serial_open(QString portName, bool wait)
{
    struct PortSettings portSettings;

    MAXD_PRINT("enter %s\n",__func__);

    portSettings = { BAUD115200, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 2 };

    my_serialport = new Win_QextSerialPort(portName, portSettings, QextSerialBase::Polling);

    if(wait)
    {
        while (!my_serialport->open(QIODevice::ReadWrite))
        {
            //MAXD_PRINT("open serial failed:%d\n",i);
            if(stopSend)
                return -1;
        }
    }
    else
    {
        if(!my_serialport->open(QIODevice::ReadWrite))
        {
            MAXD_PRINT("open serial failed\n");
            return -1;
        }
    }

    MAXD_PRINT("open serial successfully\n");

    my_serialport->setDtr();
    my_serialport->setRts();

	return 0;
}

/**
*串口发送数据
*@fd:串口描述符
*@data:待发送数据
*@datalen:数据长度
*/
int serial_write(unsigned char *data, int datalen)
{
    return my_serialport->write((const char*)data, datalen);
}


/**
*串口接收数据
*要求启动后，在pc端发送ascii文件
*/
static int serial_read_once(unsigned char *data, int datalen)
{
    int i = 0;
    qint64 cnt = 0;

    for(i=0; i<10; i++)
    {
        cnt = my_serialport->bytesAvailable();
        //MAXD_PRINT("available is:%ld\n", cnt);
        if(cnt > 0)
                break;
        max_delay(timeout_val/10);
    }

    if(i == 10)
    {
        MAXD_PRINT("read data time out\n");
        return -1;
    }

    //MAXD_PRINT("read data is ready\n");
    return  my_serialport->read((char*)data, datalen);
}

int serial_read(unsigned char *data, int datalen)
{
  int count=0, ret=0;

  //MAXD_PRINT("enter %s\n", __func__);
  do {
    ret=0;
    if(datalen<1024)
      ret=serial_read_once(data+count,datalen);
    else
      ret=serial_read_once(data+count,1024);

    if(ret<0) return count;

    count+=ret;
    datalen-=ret;
  }
  while(datalen>0);
  //MAXD_PRINT("exit %s\n", __func__);
  
  return count;
}

void serial_close()
{
  my_serialport->close();
}
