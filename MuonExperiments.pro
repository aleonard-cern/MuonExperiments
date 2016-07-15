#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T09:00:55
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MuonExperiments
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

QMAKE_CXXFLAGS += -std=c++0x

QMAKE_CXXFLAGS += -DLINUX

LIBS += -lCAENVME
