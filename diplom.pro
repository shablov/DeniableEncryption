#-------------------------------------------------
#
# Project created by QtCreator 2015-12-06T12:08:07
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = diplom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    common/dict.cpp \
    common/dictprivate.cpp \
    common/fileselectwidget.cpp \
    cryptographer.cpp \
    hashcryptographer.cpp

HEADERS  += mainwindow.h \
    include/dict.h \
    include/fileselectwidget.h \
    common/dictprivate.h \
    cryptographer.h \
    hashcryptographer.h

INCLUDEPATH += ./include


OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj

LIBS += -L$$PWD/../libarageli/lib/ -larageli

INCLUDEPATH += $$PWD/../libarageli/src/arageli
