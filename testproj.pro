#-------------------------------------------------
#
# Project created by QtCreator 2018-07-14T22:56:26
#
#-------------------------------------------------

QT       += core
QT       += core gui
QT       += serialport
QT       += network
CONFIG   += qwt
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = testproj
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += QT_DLL QWT_DLL
LIBS += -L"D:\Qt\Qt5.11.1\5.11.1\msvc2015\lib" -lqwtd
#LIBS += -L"D:\Qt\Qt5.11.1\5.11.1\msvc2015\lib" -lqwt
INCLUDEPATH += D:\Qt\Qt5.11.1\5.11.1\msvc2015\include\qwt
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    tempandhum.cpp \
    motor.cpp \
    form.cpp

HEADERS += \
        mainwindow.h \
    public.h \
    tempandhum.h \
    motor.h \
    form.h

FORMS += \
    mainwindow.ui \
    form.ui

