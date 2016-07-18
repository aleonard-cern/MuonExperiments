#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T09:00:55
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MuonExperiments
DESTDIR = .bin
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/qcustomplot.cpp

INCLUDEPATH += include

HEADERS  += include/mainwindow.h \
    include/qcustomplot.h

FORMS    += ui/mainwindow.ui

RESOURCES += \
    data/resources.qrc

QMAKE_CXXFLAGS += -std=c++0x

QMAKE_CXXFLAGS += -DLINUX

LIBS += -lCAENVME
