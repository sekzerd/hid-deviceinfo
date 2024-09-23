QT       += core gui concurrent widgets

TARGET = hid_deviceinfo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
RC_ICONS = icon.ico
VERSION = 0.0.1.0
MAKE_TARGET_COMPANY = "COMPANY"
QMAKE_TARGET_DESCRIPTION = "Tools"
QMAKE_TARGET_COPYRIGHT = "COMPANY Copyright(C) 2024"
QMAKE_TARGET_PRODUCT = "Tools"

CONFIG += warn_off no_batch

SOURCES += main.cpp
SOURCES += MainWindow.cpp
SOURCES += Config.cpp

HEADERS += Config.h
HEADERS += MainWindow.h
HEADERS += Config.h

FORMS    += MainWindow.ui

INCLUDEPATH += ..
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/../


win32: LIBS += -lhid
win32: LIBS += -lsetupapi
win32: LIBS += -lUser32

CONFIG(debug, release|debug){
   DESTDIR = $$PWD/../debug
   win32: LIBS += -L$$PWD/../debug -lHidControl_d
}else{
   DESTDIR = $$PWD/../release
   win32: LIBS += -L$$PWD/../release -lHidControl
}

RESOURCES += \
    resource.qrc

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


