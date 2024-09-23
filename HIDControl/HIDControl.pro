QT       += core gui widgets concurrent

TEMPLATE = lib

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += warn_off no_batch

HEADERS += \
    Cmd.h \
    CmdControl.h \
    CmdControlSyncTime.h \
    ConfigLoader.h \
    HidControl.h \
    Utils.h \
    hidapi.h

SOURCES += \
    CmdControl.cpp \
    CmdControlSyncTime.cpp \
    ConfigLoader.cpp \
    HidControl.cpp \
    Utils.cpp \
    hidapi.c

LIBS += -lhid
LIBS += -lSetupApi
LIBS += -lUser32

CONFIG += static
#CONFIG += plugin

CONFIG(debug, release|debug){
   TARGET = HidControl_d
   DESTDIR = $$PWD/../debug
}else{
   TARGET = HidControl
   DESTDIR = $$PWD/../release
}

CONFIG(debug, release|debug){
    OBJECTS_DIR += $$PWD/debug/obj
    MOC_DIR     += $$PWD/debug/moc
    RCC_DIR     += $$PWD/debug/rcc
    UI_DIR      += $$PWD/debug/ui
}else{
    OBJECTS_DIR += $$PWD/release/obj
    MOC_DIR     += $$PWD/release/moc
    RCC_DIR     += $$PWD/release/rcc
    UI_DIR      += $$PWD/release/ui
}


