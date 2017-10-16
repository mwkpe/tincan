#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T10:46:53
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network
QT += charts

CONFIG += c++1z
CONFIG(release, debug|release) {
    CONFIG += optimize_full
}

TARGET = tincan
TEMPLATE = app

INCLUDEPATH += $$PWD/src \
    $$PWD/ext \
    C:/dev/boost_1_65_1 \
    C:/dev/asio-1.10.6/include

LIBS += -lstdc++fs \
    -lws2_32

DEFINES += ASIO_STANDALONE

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
    src/ui/mainwindow.cpp \
    src/util.cpp \
    src/file/dbcparser.cpp \
    src/models/canbusmodel.cpp \
    src/models/canframeitem.cpp \
    src/models/treeitem.cpp \
    src/models/treemodel.cpp \
    src/network/canreceiver.cpp \
    src/models/bussignalitem.cpp \
    src/tincan/canbus.cpp \
    src/tincan/translate.cpp \
    ext/fmt/fmtlib.cc \
    src/models/bussignaldefitem.cpp \
    src/models/canbusdefmodel.cpp \
    src/models/canframedefitem.cpp \
    src/tincan/busdefreader.cpp \
    src/tincan/busdefwriter.cpp \
    src/network/udpasyncreceiver.cpp \
    src/tincan/signalutil.cpp \
    src/tincan/cantracer.cpp

HEADERS += src/ui/mainwindow.h \
    src/util.h \
    src/file/dbcfile.h \
    src/file/dbcparser.h \
    src/models/canbusmodel.h \
    src/models/canframeitem.h \
    src/models/treeitem.h \
    src/models/treemodel.h \
    src/network/canrawframe.h \
    src/network/canreceiver.h \
    src/models/bussignalitem.h \
    src/tincan/bussignal.h \
    src/tincan/bussignaldef.h \
    src/tincan/canbus.h \
    src/tincan/canbusdef.h \
    src/tincan/canframe.h \
    src/tincan/canframedef.h \
    src/tincan/translate.h \
    ext/fmt/fmtlib.h \
    src/models/bussignaldefitem.h \
    src/models/canbusdefmodel.h \
    src/models/canframedefitem.h \
    src/tincan/busdefreader.h \
    src/tincan/busdefwriter.h \
    src/tincan/errors.h \
    src/network/udpasyncreceiver.h \
    src/network/udpsocketguard.h \
    src/tincan/signalutil.h \
    src/tincan/cantracer.h

FORMS += ui/mainwindow.ui
