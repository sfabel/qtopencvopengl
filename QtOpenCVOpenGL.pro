#-------------------------------------------------
#
# Project created by QtCreator 2011-02-16T20:58:21
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = QtOpenCVOpenGL
TEMPLATE = app

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui

INCLUDEPATH += /usr/local/include

SOURCES += main.cpp\
        opencvgl.cpp

HEADERS  += opencvgl.hpp
