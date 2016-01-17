#-------------------------------------------------
#
# Project created by QtCreator 2015-12-19T13:15:56
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ./../bin/application
TEMPLATE = app

OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj

SOURCES += main.cpp\
	mainwindow.cpp

HEADERS  += mainwindow.h
INCLUDEPATH += ./../include

unix:!macx: LIBS += -L$$PWD/../lib/ -lcryptograph
INCLUDEPATH += ./../include/cryptograph

unix:!macx: LIBS += -L$$PWD/../lib/ -larageli
INCLUDEPATH += $$PWD/../include/arageli

unix:!macx: LIBS += -L$$PWD/.m./../lib/ -lintel_aes
INCLUDEPATH += $$PWD/../../include/intel_aes

unix:!macx: LIBS += -L$$PWD/../lib/ -lgui
INCLUDEPATH += $$PWD/../include/gui

unix:!macx: LIBS += -L$$PWD/../lib/ -lcommon
INCLUDEPATH += $$PWD/../include/common
