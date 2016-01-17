TEMPLATE = subdirs

SUBDIRS = libcommon \
	libgui \
	libcryptograph

libgui.depends = libcommon
libcryptograph.depends = libgui
