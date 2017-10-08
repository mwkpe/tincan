#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T10:46:53
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

CONFIG += c++1z

TARGET = tincan
TEMPLATE = app

INCLUDEPATH += C:/dev/boost_1_65_1
LIBS += -lstdc++fs

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/canbus.cpp \
    src/util.cpp \
    src/file/dbcparser.cpp \
    src/models/canbusmodel.cpp \
    src/models/canframeitem.cpp \
    src/models/treeitem.cpp \
    src/models/treemodel.cpp \
    src/network/canreceiver.cpp \
    src/network/udpreceiver.cpp \
    src/models/bussignalitem.cpp

HEADERS += src/mainwindow.h \
    src/bussignal.h \
    src/canbus.h \
    src/canframe.h \
    src/util.h \
    src/file/dbcfile.h \
    src/file/dbcparser.h \
    src/models/canbusmodel.h \
    src/models/canframeitem.h \
    src/models/treeitem.h \
    src/models/treemodel.h \
    src/network/canrawframe.h \
    src/network/canreceiver.h \
    src/network/udpreceiver.h \
    src/models/bussignalitem.h


FORMS += ui/mainwindow.ui
