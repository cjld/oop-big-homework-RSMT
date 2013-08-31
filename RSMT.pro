#-------------------------------------------------
#
# Project created by QtCreator 2013-06-28T15:45:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11

TARGET = RSMT
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h\
        fly.h \
    fly_pre.h

FORMS    += widget.ui
