#-------------------------------------------------
#
# Project created by QtCreator 2015-12-15T00:18:06
#
#-------------------------------------------------

QT       += core

TARGET = cryptograph
TEMPLATE = app

OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj

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
