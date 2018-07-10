#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <Qfile>
#include <QTextStream>
#include <QDateTime>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    static bool fileOpened = false;
    static QTextStream text_stream;

    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }

    if(false == fileOpened)
    {
        static QFile file(QString("%1%2").arg(QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")).arg(".txt"));

        file.open(QIODevice::WriteOnly);
        text_stream.setDevice(&file);
        fileOpened = true;
    }

    //保存输出相关信息到指定文件
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString message = QString("%1 %2 %3").arg(text).arg(current_date_time).arg(msg);
    text_stream << message;
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //qInstallMessageHandler(outputMessage);

    w.show();

    return a.exec();
}
