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
    grayscalefilter.cpp \
    averagefilter.cpp \
    contrastfilter.cpp \
    rosenfeldfilter.cpp \
    qualitychecker.cpp \
    colorparser.cpp \
    transformfilter.cpp \
    fft.cpp \
    complex.cpp \
    displaywindow.cpp \
    transformwindow.cpp

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
    grayscalefilter.h \
    averagefilter.h \
    contrastfilter.h \
    rosenfeldfilter.h \
    qualitychecker.h \
    colorparser.h \
    transformfilter.h \
    fft.h \
    complex.h \
    complexarray.h \
    displaywindow.h \
    transformwindow.h

FORMS    += mainwindow.ui \
    photowindow.ui \
    transformwindow.ui
