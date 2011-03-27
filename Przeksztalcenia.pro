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
    dockwidget.cpp \
    negativefilter.cpp \
    filterinterface.cpp \
    convolutionfilter.cpp \
	kernelvaluesdialog.cpp \
	medianfilter.cpp \
    sizedialog.cpp \
    slidingvaluedialog.cpp \
    brightnessfilter.cpp \
    grayscalefilter.cpp

HEADERS  += mainwindow.h \
    photowindow.h \
    dockwidget.h \
    filterinterface.h \
    negativefilter.h \
    convolutionfilter.h \
	kernelvaluesdialog.h \
	medianfilter.h \
    sizedialog.h \
    slidingvaluedialog.h \
    brightnessfilter.h \
    grayscalefilter.h

FORMS    += mainwindow.ui \
    photowindow.ui
