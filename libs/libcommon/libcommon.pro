#-------------------------------------------------
#
# Project created by QtCreator 2015-01-16T12:08:07
#
#-------------------------------------------------

QT       += core xml xmlpatterns

TARGET = ./../../lib/common
TEMPLATE = lib

SOURCES += \
	dict.cpp \
	dictprivate.cpp

HEADERS  += dictprivate.h \
	./../../include/common\dict.h

INCLUDEPATH += ./../../include \
			   ./../../include/common
