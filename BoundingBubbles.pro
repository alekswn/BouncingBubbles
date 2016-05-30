#-------------------------------------------------
#
# Project created by QtCreator 2016-05-12T21:19:20
#
#-------------------------------------------------

CONFIG += c++14
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BoundingBubbles
TEMPLATE = app


SOURCES += main.cpp\
    model.cpp \
    presenter.cpp \
    bubble.cpp \
    view.cpp

HEADERS  += \
    model.h \
    presenter.h \
    bubble.h \
    view.h \
    utils.h

FORMS    +=

CONFIG += mobility
MOBILITY = 

