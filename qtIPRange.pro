#-------------------------------------------------
#
# Project created by QtCreator 2017-07-17T13:55:33
#
#-------------------------------------------------

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(3rdparty/qtxlsx/src/xlsx/qtxlsx.pri)

TARGET = qtIPRange
TEMPLATE = app
RC_ICONS = qtIPRange.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    cimportexceldialog.cpp \
    cimportexcelitemdelegate.cpp \
        main.cpp \
        cmainwindow.cpp \
    cipaddress.cpp \
    ciprange.cpp \
    ciprangeitemdelegate.cpp \
    clocation.cpp \
    cnewiprange.cpp \
    cfinddialogiprange.cpp \
    cfinddialogipaddress.cpp \
    caddlocationdialog.cpp \
    caddcitydialog.cpp \
    csplashscreen.cpp

HEADERS += \
    cimportexceldialog.h \
    cimportexcelitemdelegate.h \
        cmainwindow.h \
    cipaddress.h \
    ciprange.h \
    ciprangeitemdelegate.h \
    clocation.h \
    cnewiprange.h \
    cfinddialogiprange.h \
    cfinddialogipaddress.h \
    caddlocationdialog.h \
    caddcitydialog.h \
    csplashscreen.h

FORMS += \
    cimportexceldialog.ui \
        cmainwindow.ui \
    cnewiprange.ui \
    cfinddialogiprange.ui \
    cfinddialogipaddress.ui \
    caddlocationdialog.ui \
    caddcitydialog.ui

DISTFILES += \
    README.md \
    qtIPRange.ico \
    Doxyfile

RESOURCES += \
    qtiprange.qrc
