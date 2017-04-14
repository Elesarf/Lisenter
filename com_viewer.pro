#-------------------------------------------------
#
# Project created by QtCreator 2017-03-15T11:33:19
#
#-------------------------------------------------
linux-g++{
 message($$QMAKESPEC)

QT       += core gui
QT          += serialport
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11


TARGET = com_viewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    camreader.cpp \
    udpsocket.cpp \
    my_tcpsocket.cpp

HEADERS  += mainwindow.h \
    camreader.h \
    udpsocket.h \
    my_tcpsocket.h \
settings.h

FORMS    += mainwindow.ui
}


linux-rasp-pi-g++{
 message($$QMAKESPEC)

QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11


TARGET = com_viewer
TEMPLATE = app
target.path = /usr/bin
INSTALLS += target


SOURCES += main.cpp\
        mainwindow.cpp \
    udpsocket.cpp \
    my_tcpsocket.cpp

HEADERS  += mainwindow.h \
    udpsocket.h \
    my_tcpsocket.h \
settings.h

FORMS    += mainwindow.ui
}

HEADERS += \
    customscene.h

SOURCES += \
    customscene.cpp

