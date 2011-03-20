#-------------------------------------------------
#
# Project created by QtCreator 2011-03-08T21:16:26
#
#-------------------------------------------------

QT       += core gui

CONFIG(debug, debug | release){
    CONFIG += console
}

TARGET = Przeksztalcenia
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    photowindow.cpp \
    dockwidget.cpp

HEADERS  += mainwindow.h \
    photowindow.h \
    dockwidget.h

FORMS    += mainwindow.ui \
    photowindow.ui
