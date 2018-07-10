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

#ifndef MASTERTHREAD_H
#define MASTERTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

enum MasterStatus
{
    START_DOWN = 0,
    FACTORY_WRITE,
    FACTORY_READ
};
//! [0]
class MasterThread : public QThread
{
    Q_OBJECT

public:
    explicit MasterThread(QObject *parent = nullptr);
    ~MasterThread();

    void startdown(const QString &portName, const QString &fileName, const bool crk, const bool bin);
    void factoryWrite(const QString &portName, const QString &sn, const QString &deviceId, const QString &apiSecret);
    void factoryRead(const QString &portName);
    void stopdown();
    void emitResponse(const char *fmt, ...);

signals:
    void response(const QString &s);
    void finish(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);
    void factoryReadResponse(const QString &s);

private:
    void run() override;
    unsigned char cal_lrc(QByteArray &frame, unsigned int len);
    void buildFrame(const QString &content, QByteArray &frame);
    int readFrame(QByteArray &frame);

    QString m_portName;
    QString m_fileName;
    QString m_request;
    bool m_crk;
    bool m_bin;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;
    enum MasterStatus masterStatus;
    QString m_sn;
    QString m_deviceId;
    QString m_apiSecret;
};
//! [0]

#endif // MASTERTHREAD_H
