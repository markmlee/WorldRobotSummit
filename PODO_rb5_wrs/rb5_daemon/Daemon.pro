QT  += sql

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

CONFIG(debug, debug|release) {
    DESTDIR = ../../exe
} else {
    DESTDIR = ../../exe
}

QMAKE_CXXFLAGS += -D_GNU_SOURCE -D_REENTRANT -D__COBALT__

#QMAKE_LFLAGS += /usr/lib/xenomai/bootstrap.o -Wl,--wrap=main -Wl,--dynamic-list=/usr/lib/dynlist.ld
QMAKE_LFLAGS += -L/usr/lib -lpthread -lrt

LIBS    += \
    -lpthread \
    -lrt \
    -lpcan


SOURCES += main.cpp \
    BasicHeader/RBRawLAN.cpp \
    device/RBDataBase.cpp \
    device/RBMotorController.cpp \
    device/RBFTSensor.cpp \
    device/JoyStick/RBJoystick.cpp \
    RBCAN.cpp \
    OMNImove.cpp

HEADERS += \
    BasicHeader/RBLog.h \
    BasicHeader/RBDataType.h \
    BasicHeader/JointInformation.h \
    BasicHeader/RBRawLAN.h \
    device/RBDataBase.h \
    device/RBMotorController.h \
    device/RBFTSensor.h \
    device/JoyStick/joystickvariable.h \
    device/JoyStick/joystickclass.h \
    RBCAN.h \
    OMNImove.h


