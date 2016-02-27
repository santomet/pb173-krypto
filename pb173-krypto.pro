#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T13:07:26
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = pb173-krypto
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lQt5Concurrent -lmbedtls -lmbedcrypto

SOURCES += main.cpp \
    mainconsole.cpp

HEADERS += \
    mainconsole.h
