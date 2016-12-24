#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T07:44:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelloKinect
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        dialog.cpp \
    kinectthread.cpp \
    kinectobj.cpp \
    kinectwidget.cpp \
    kinectdepthobj.cpp \
    kinectfactory.cpp \
    kinectcolorobj.cpp \
    kinectgrayobj.cpp \
    kinectbodyindex.cpp \
    kinectgreenscreeninbl.cpp

HEADERS  += dialog.h \
    stdinc.h \
    Kinect.h \
    Kinect.Face.h \
    Kinect.INPC.h \
    Kinect.VisualGestureBuilder.h \
    NuiKinectFusionApi.h \
    NuiKinectFusionBase.h \
    NuiKinectFusionCameraPoseFinder.h \
    NuiKinectFusionColorVolume.h \
    NuiKinectFusionDepthProcessor.h \
    NuiKinectFusionVolume.h \
    kinectthread.h \
    kinectobj.h \
    kinectwidget.h \
    kinectdepthobj.h \
    kinectframeprotocol.h \
    kinectfactory.h \
    kinectcolorobj.h \
    kinectgrayobj.h \
    kinectbodyindex.h \
    kinectgreenscreeninbl.h


FORMS    += dialog.ui

win32: LIBS += -L$$PWD/Kinect/Lib/x64/ -lKinect20
win32: LIBS += -L$$PWD/Kinect/Lib/ -lUser32

INCLUDEPATH += $$PWD/Kinect/inc
DEPENDPATH += $$PWD/Kinect/inc
