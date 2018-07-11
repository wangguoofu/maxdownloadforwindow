#-------------------------------------------------
#
# Project created by QtCreator 2018-06-28T14:51:06
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MaxDownLoader
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        BootloaderScp.cpp \
        bufferfile.cpp \
        linefile.cpp \
        lineop.cpp \
        max_bin.cpp \
        max_debug.cpp \
        max_delay.cpp \
        max_rambin.cpp \
        ramfile.cpp \
        scpcmd.cpp \
        ScpCmdHdr.cpp \
        serial.cpp \
        serial_sender.cpp \
        masterthread.cpp \
        qextserialbase.cpp \
        qextserialport.cpp \
        win_qextserialport.cpp \
        qextserialenumerator.cpp

HEADERS += \
        mainwindow.h \
        BootloaderScp.h \
        bufferfile.h \
        linefile.h \
        lineop.h \
        max_debug.h \
        max_delay.h \
        max_download.h \
        ramfile.h \
        ScpCmd.h \
        ScpCmdHdr.h \
        serial.h \
        serial_sender.h \
        masterthread.h \
        qextserialbase.h \
        qextserialport.h \
        win_qextserialport.h \
        qextserialenumerator.h

FORMS += \
        mainwindow.ui

DEFINES += MAX_DOWNLOAD_RAMBIN
