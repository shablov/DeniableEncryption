#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T00:18:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cryptograph
TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ./../../include

unix:!macx: LIBS += -L$$PWD/../../lib/ -larageli
unix:!macx: LIBS += -L$$PWD/../../lib/ -lintel_aes
unix:!macx: LIBS += -L$$PWD/../../lib/ -lcryptograph

INCLUDEPATH += $$PWD/../../include/arageli
INCLUDEPATH += $$PWD/../../include/intel_aes
INCLUDEPATH += $$PWD/../../include/cryptograph

DEPENDPATH += $$PWD/../../include/arageli
DEPENDPATH += $$PWD/../../include/intel_aes
DEPENDPATH += $$PWD/../../include/cryptograph
