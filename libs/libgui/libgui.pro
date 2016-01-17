#-------------------------------------------------
#
# Project created by QtCreator 2015-01-16T12:10:07
#
#-------------------------------------------------

QT       += core widgets

TARGET = ./../../lib/gui
TEMPLATE = lib

SOURCES += guitools.cpp \
		   widgets/checkedcombobox.cpp \
		   widgets/colorlabel.cpp \
		   widgets/fileselectwidget.cpp \
		   widgets/ledwidget.cpp

HEADERS += ./../../include/gui/guitools.h \
		   ./../../include/gui/widgets/checkedcombobox.h \
		   ./../../include/gui/widgets/colorlabel.h \
		   ./../../include/gui/widgets/fileselectwidget.h \
		   ./../../include/gui/widgets/ledwidget.h

INCLUDEPATH += ./../../include \
			   ./../../include/gui
