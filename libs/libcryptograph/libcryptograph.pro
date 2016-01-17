#-------------------------------------------------
#
# Project created by QtCreator 2015-12-06T12:08:07
#
#-------------------------------------------------

QT       += core widgets

TARGET = ./../../lib/cryptograph
TEMPLATE = lib

CONFIG += c++11

OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj

SOURCES += \
	cryptographer.cpp \
	hashcryptographer.cpp \
	aescryptographer.cpp \
	datadevice.cpp \
    datasourcewidget.cpp \
    keywidget.cpp

HEADERS  += \
	../../include/cryptograph/cryptographer.h \
	../../include/cryptograph/hashcryptographer.h \
	../../include/cryptograph/aescryptographer.h \
	../../include/cryptograph/tools.h \
	../../include/cryptograph/datadevice.h \
    ../../include/cryptograph/datasourcewidget.h \
    ../../include/cryptograph/keywidget.h

INCLUDEPATH += ./../../include \
				./../../include/cryptograph

unix:!macx: LIBS += -L$$PWD/../../lib/ -larageli
INCLUDEPATH += $$PWD/../../include/arageli
DEPENDPATH += $$PWD/../../include/arageli

unix:!macx: LIBS += -L$$PWD/../../lib/ -lintel_aes
INCLUDEPATH += $$PWD/../../include/intel_aes
DEPENDPATH += $$PWD/../../include/intel_aes

unix:!macx: LIBS += -L$$PWD/../../lib/ -lcommon
INCLUDEPATH += $$PWD/../../include/common
DEPENDPATH += $$PWD/../../include/common

unix:!macx: LIBS += -L$$PWD/../../lib/ -lgui
INCLUDEPATH += $$PWD/../../include/gui
DEPENDPATH += $$PWD/../../include/gui
