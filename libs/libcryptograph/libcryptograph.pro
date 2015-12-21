#-------------------------------------------------
#
# Project created by QtCreator 2015-12-06T12:08:07
#
#-------------------------------------------------

QT       += core

TARGET = ./../../lib/cryptograph
TEMPLATE = lib

OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj

SOURCES += \
    cryptographer.cpp \
    hashcryptographer.cpp \
	aescryptographer.cpp \
    datadevice.cpp

HEADERS  += \
    ./../../include/cryptograph/cryptographer.h \
    ./../../include/cryptograph/hashcryptographer.h \
    ./../../include/cryptograph/aescryptographer.h \
	./../../include/cryptograph/tools.h \
	./../../include/cryptograph/datadevice.h

INCLUDEPATH += ./../../include \
               ./../../include/cryptograph \

unix:!macx: LIBS += -L$$PWD/../../lib/ -larageli
INCLUDEPATH += $$PWD/../../include/arageli
DEPENDPATH += $$PWD/../../include/arageli

unix:!macx: LIBS += -L$$PWD/.m./../lib/ -lintel_aes
INCLUDEPATH += $$PWD/../../include/intel_aes
DEPENDPATH += $$PWD/../../include/intel_aes
