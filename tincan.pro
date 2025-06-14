#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T10:46:53
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += network

CONFIG += c++17
CONFIG(release, debug|release) {
    CONFIG += optimize_full
}

TARGET = tincan
TEMPLATE = app

INCLUDEPATH += $$PWD/src \
    $$PWD/ext \
    C:/libs/boost_1_88_0 \
    C:/libs/asio-1.30.2/include

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
    src/models/cansignaldefitem.cpp \
    src/models/cansignalitem.cpp \
    src/models/valuedefitem.cpp \
    src/models/valuedefmodel.cpp \
    src/ui/mainwindow.cpp \
    src/ui/treeviewdialog.cpp \
    src/util.cpp \
    src/file/dbcparser.cpp \
    src/models/canbusmodel.cpp \
    src/models/canframeitem.cpp \
    src/models/treeitem.cpp \
    src/models/treemodel.cpp \
    src/tincan/canbus.cpp \
    src/tincan/simulator.cpp \
    src/tincan/translate.cpp \
    ext/fmt/fmtlib.cc \
    src/models/canbusdefmodel.cpp \
    src/models/canframedefitem.cpp \
    src/network/udpasyncreceiver.cpp \
    src/tincan/signalutil.cpp \
    src/tincan/cantracer.cpp \
    src/network/canudpreceiver.cpp \
    src/file/jsonreader.cpp \
    src/file/jsonwriter.cpp \
    src/file/dbcwriter.cpp \
    src/file/dbcmetaparser.cpp

HEADERS += src/ui/mainwindow.h \
    src/models/cansignaldefitem.h \
    src/models/cansignalitem.h \
    src/models/treeitemid.h \
    src/models/valuedefitem.h \
    src/models/valuedefmodel.h \
    src/network/dummyreceiver.h \
    src/tincan/canrawframe.h \
    src/tincan/cansignal.h \
    src/tincan/cansignaldef.h \
    src/ui/treeviewdialog.h \
    src/util.h \
    src/file/dbcfile.h \
    src/file/dbcparser.h \
    src/models/canbusmodel.h \
    src/models/canframeitem.h \
    src/models/treeitem.h \
    src/models/treemodel.h \
    src/tincan/canbus.h \
    src/tincan/simulator.h \
    src/tincan/canbusdef.h \
    src/tincan/canframe.h \
    src/tincan/canframedef.h \
    src/tincan/translate.h \
    ext/fmt/fmtlib.h \
    src/models/canbusdefmodel.h \
    src/models/canframedefitem.h \
    src/tincan/errors.h \
    src/network/udpasyncreceiver.h \
    src/network/udpsocketguard.h \
    src/tincan/signalutil.h \
    src/tincan/cantracer.h \
    src/network/canudpreceiver.h \
    src/file/jsonreader.h \
    src/file/jsonwriter.h \
    src/file/dbcwriter.h \
    src/file/dbcmetaparser.h

FORMS += ui/mainwindow.ui \
    ui/treeviewdialog.ui
