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
    src/udpreceiver.cpp \
    src/canreceiver.cpp \
    src/dbcparser.cpp \
    src/util.cpp \
    src/treeitem.cpp \
    src/frameitem.cpp \
    src/signalitem.cpp \
    src/framemodel.cpp \
    src/treemodel.cpp \
    src/canbus.cpp

HEADERS += src/mainwindow.h \
    src/udpreceiver.h \
    src/canreceiver.h \
    src/dbcparser.h \
    src/util.h \
    src/dbcfile.h \
    src/treeitem.h \
    src/frameitem.h \
    src/signalitem.h \
    src/framemodel.h \
    src/treemodel.h \
    candata.h \
    src/canbus.h \
    src/canframe.h

FORMS += ui/mainwindow.ui
