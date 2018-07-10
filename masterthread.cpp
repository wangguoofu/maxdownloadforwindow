/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QFile>
#include <QTime>
#include <QDebug>

#include "masterthread.h"
#include "serial_sender.h"
#include "max_debug.h"
#include "serial.h"

MasterThread::MasterThread(QObject *parent) :
    QThread(parent)
{
    serial_sender_stop(false);
}

MasterThread::~MasterThread()
{
    serial_sender_stop(true);
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

void MasterThread::startdown(const QString &portName, const QString &fileName, const bool crk, const bool bin)
{
    const QMutexLocker locker(&m_mutex);

    qDebug() <<"enter "<<__func__<<endl;
    m_portName = portName;
    m_fileName = fileName;
    m_crk = crk;
    m_bin = bin;
    serial_sender_stop(false);

    masterStatus =  START_DOWN;
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

void MasterThread::factoryWrite(const QString &portName, const QString &sn, const QString &deviceId, const QString &apiSecret)
{
    const QMutexLocker locker(&m_mutex);
    qDebug() <<"enter "<<__func__<<endl;
    m_sn = sn;
    m_deviceId = deviceId;
    m_apiSecret = apiSecret;
    m_portName = portName;
    serial_sender_stop(false);

    masterStatus = FACTORY_WRITE;
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

void MasterThread::factoryRead(const QString &portName)
{
    const QMutexLocker locker(&m_mutex);
    qDebug() <<"enter "<<__func__<<endl;
    m_portName = portName;
    serial_sender_stop(false);

    masterStatus = FACTORY_READ;
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

void MasterThread::stopdown()
{
    serial_sender_stop(true);
}

unsigned char MasterThread::cal_lrc(QByteArray &frame, unsigned int len)
{
    unsigned int i;
    unsigned char lrc = 0;
    const char *pData = frame.data();

    for(i=0; i<len; i++)
        lrc^= pData[i];

    return lrc;
}

int MasterThread::readFrame(QByteArray &frame)
{
    int ret = 0;
    unsigned char lrc = 0;

    ret = serial_read((unsigned char*)(frame.data()), 1);
    unsigned char startOfFrame = frame.data()[0];
    if((1 != ret)||startOfFrame!=0xAA)
    {
        qDebug() <<"read ack failed"<<endl;
        return -1;
    }
    ret = serial_read((unsigned char*)(frame.data())+1, 4);//read header

    unsigned char low = frame.data()[2];
    unsigned char high = frame.data()[3];
    unsigned int contentLen = (high<<0x08)|low;
    qDebug() <<"ack contentLen:"<<contentLen<<endl;
    ret = serial_read((unsigned char*)(frame.data())+5, contentLen);
    lrc = cal_lrc(frame, contentLen+5);
    qDebug()<<"ack:lrc= "<<lrc<<endl;
    if(lrc)
    {
        qDebug() <<"lrc error"<<endl;
        return -1;
    }

    return contentLen+5;
}

void MasterThread::buildFrame(const QString &content, QByteArray &frame)
{
    int contenLen = 0;
    unsigned char lrc = 0;

    contenLen = content.size()+1;//last is lrc
    frame.append((char)0xAA);//start of frame
    frame.append((char)0x00);//reserve
    frame.append((char)(contenLen&0xff));//len
    frame.append((char)((contenLen>>0x08)&0xff));//len
    frame.append((char)0x00);//seq
    frame.append(content);
    lrc = cal_lrc(frame, content.size()+5);
    qDebug()<<__func__<<":lrc= "<<lrc<<endl;
    frame.append(lrc);
}

void MasterThread::run()
{
    int ret = 0;
    while (!m_quit) {
        m_mutex.lock();
        switch(masterStatus)
        {
            case START_DOWN:
            {
                QFile file(m_fileName);
                QByteArray byteArray;
                const char *pBin;
                unsigned int binLen;

                if(!file.open(QIODevice::ReadOnly)) {
                    qDebug()<<"Can't open the file!"<<endl;
                }

                byteArray = file.readAll();
                pBin = byteArray.data();
                binLen = byteArray.size();

                if(!m_crk&&!m_bin)
                    qDebug()<<"please select crk or bin"<<endl;

                pMasterThread = this;
                if(m_crk)
                {
                    emit response("start down crk");
                    ret = serial_sender_main(m_portName, (const unsigned char*)pBin, binLen, "crk");
                }
                if(m_bin)
                {
                    emit response("start down bin");
                    ret = serial_sender_main(m_portName, (const unsigned char*)pBin, binLen, "bin");
                }

                if(!ret)
                {
                    emit response("down successfully");
                    emit finish("success");
                }
                else
                {
                    emit response("down failed");
                    emit finish("fail");
                }
                break;
            }

            case FACTORY_WRITE:
            {
                QByteArray frame;
                QByteArray *ack;
                int ret;

                if(serial_open(m_portName, false))
                {
                    qDebug() <<"serial open failed"<<endl;
                    emit finish("fail");
                    break;
                }

                QString endOfLine = "\n";
                QString content=QString("sn:") + m_sn + endOfLine;
                content = content + "device_id:" + m_deviceId + endOfLine;
                content = content + "api_secret:" + m_apiSecret + endOfLine;
                content = content + "method:factory_write_device_info\n";
                qDebug() <<"content:"<<content<<endl;

                buildFrame(content, frame);
                serial_write((unsigned char*)(frame.data()), frame.size());

                serial_settimeout(1000000);
                ack = new QByteArray(2048, 0x00);
                ret = readFrame(*ack);
                if(ret <= 0)
                {
                   emit finish("fail");
                   serial_close();
                   delete ack;
                   break;
                }

                if(ack->contains("state:success"))
                    emit finish("success");
                else
                    emit finish("fail");

                serial_close();
                delete ack;
                break;
            }

            case FACTORY_READ:
            {
                QByteArray frame;
                QByteArray *ack;
                int ret;

                if(serial_open(m_portName, false))
                {
                    qDebug() <<"serial open failed"<<endl;
                    emit finish("fail");
                    break;
                }

                QString endOfLine = "\n";
                QString content = QString("");
                content = content + "method:factory_read_device_info\n";
                qDebug() <<"content:"<<content<<endl;

                buildFrame(content, frame);
                serial_write((unsigned char*)(frame.data()), frame.size());

                serial_settimeout(1000000);
                ack = new QByteArray(2048, 0x00);
                ret = readFrame(*ack);
                if(ret <= 0)
                {
                   emit finish("fail");
                }
                else
                {
                    ack->data()[ret-1] = 0x00;
                    emit factoryReadResponse(QString(ack->data()+5));
                }

                serial_close();
                delete ack;
                break;
            }
        }
        m_cond.wait(&m_mutex);
        m_mutex.unlock();
    }
}

void MasterThread::emitResponse(const char *fmt, ...)
{
    #define RESPONSE_BUFF_SIZE 2048
    static char s_pResponseBuff[RESPONSE_BUFF_SIZE];

    char *p = s_pResponseBuff;
    va_list args;

    memset(s_pResponseBuff,0,sizeof(s_pResponseBuff));

    va_start(args, fmt);
    vsprintf(p, fmt, args);
    va_end(args);

    emit response(s_pResponseBuff);
}
