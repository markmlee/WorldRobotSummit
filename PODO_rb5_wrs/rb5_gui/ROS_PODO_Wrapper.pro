#-------------------------------------------------
#
# Project created by QtCreator 2019-04-09T15:26:38
#
#-------------------------------------------------

QT       += core gui network sql opengl


INCLUDEPATH += \
    ../../cobot_podo/arch
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = wrapper
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11
LIBS += -lrt

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    LAN/BaseLAN/RBTCPClient.cpp \
    LAN/BaseLAN/RBTCPServer.cpp \
    LAN/CANconnect.cpp \
    LAN/RB5connect.cpp \
    LAN/ROSconnect.cpp \
    StatusDialog.cpp \
    CANDialog.cpp \
    ManualDialog.cpp


HEADERS += \
        mainwindow.h \
    LAN/BaseLAN/RBLANCommon.h \
    LAN/BaseLAN/RBLog.h \
    LAN/BaseLAN/RBTCPClient.h \
    LAN/BaseLAN/RBTCPServer.h \
    LAN/CANconnect.h \
    LAN/RB5connect.h \
    LAN/ROSconnect.h \
    DataStructure/rb5data.h \
    DataStructure/rosdata.h \
    StatusDialog.h \
    CANDialog.h \
    ManualDialog.h


FORMS += \
        mainwindow.ui \
    StatusDialog.ui \
    CANDialog.ui \
    ManualDialog.ui

RESOURCES += \
        1.png \
        2.png

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/../build
} else {
    DESTDIR = $$PWD/../build
}

DISTFILES +=
