#-------------------------------------------------
#
# Project created by QtCreator 2014-11-15T17:36:21
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VimanaGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    checksum.cpp \
    protocol.cpp \
    telegram.cpp

HEADERS  += mainwindow.h \
    CommandLibrary.h \
    checksum.h \
    protocol.h \
    telegram.h

FORMS    += mainwindow.ui
