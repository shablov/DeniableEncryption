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
	mainwindow.cpp \
	common/dictprivate.cpp \
	common/dict.cpp \
	gui/guitools.cpp \
	gui/widgets/checkedcombobox.cpp \
	gui/widgets/colorlabel.cpp \
	gui/widgets/fileselectwidget.cpp \
	gui/widgets/ledwidget.cpp \
    gui/widgets/keywidget.cpp \
    gui/widgets/datasourcewidget.cpp

HEADERS  += mainwindow.h \
	common/dictprivate.h \
	common/dict.h \
	gui/guitools.h \
	gui/widgets/checkedcombobox.h \
	gui/widgets/colorlabel.h \
	gui/widgets/fileselectwidget.h \
	gui/widgets/ledwidget.h \
    gui/widgets/keywidget.h \
    gui/widgets/datasourcewidget.h


unix:!macx: LIBS += -L$$PWD/../lib/ -lcryptograph
INCLUDEPATH += ./../include \
			   ./../include/cryptograph \

unix:!macx: LIBS += -L$$PWD/../lib/ -larageli
INCLUDEPATH += $$PWD/../include/arageli

unix:!macx: LIBS += -L$$PWD/.m./../lib/ -lintel_aes
INCLUDEPATH += $$PWD/../../include/intel_aes
