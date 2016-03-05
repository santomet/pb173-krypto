#-------------------------------------------------
#
# Project created by QtCreator 2016-02-22T13:07:26
#
#-------------------------------------------------

QT       += core

QT       -= gui

CONFIG   += console
#CONFIG  += c++11
CONFIG   -= app_bundle

QMAKE_CXX = g++-4.8
QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app

LIBS += -lmbedtls -lmbedcrypto

SOURCES += \
    src/mainconsole.cpp \

HEADERS += \
    src/mainconsole.h


DISTFILES += \
    .travis.yml

configMain {

TARGET = pb173-krypto

SOURCES += src/main.cpp
}

configTest {

TARGET = pb173-krypto-test

SOURCES += src/tests.cpp

}

