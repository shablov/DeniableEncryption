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
    hashcryptographer.cpp \
    aescryptographer.cpp

HEADERS  += mainwindow.h \
    include/dict.h \
    include/fileselectwidget.h \
    common/dictprivate.h \
    cryptographer.h \
    hashcryptographer.h \
    aescryptographer.h \
    tools.h

INCLUDEPATH += ./include


OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = obj

LIBS += -L$$PWD/libarageli/lib/ -larageli \
	-L$$PWD/intel_aes_lib/lib/ -lintel_aes

INCLUDEPATH += $$PWD/libarageli/src/arageli \
	-L$$PWD/intel_aes_lib/include

!exists(libarageli/lib/libarageli.a) {
    message("configure and make libarageli...")
    system(cd libarageli \
    && mkdir lib \
    && make \
    && cd ..)
}
exists(libarageli/lib/libarageli.a) {
    message("libarageli found")
}

!exists(intel_aes_lib/lib/libintel_aes.so) {
    message("configure and make lib_intel_aes...")
    system(cd intel_aes_lib \
    && ./mk_lnx_lib.sh \
    && cd ..)
}
exists(intel_aes_lib/lib/libintel_aes.so) {
    message("lib_intel_aes found")
}
