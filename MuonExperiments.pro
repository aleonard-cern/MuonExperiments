#-------------------------------------------------
#
# Project created by QtCreator 2016-01-19T09:00:55
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MuonExperiments
DESTDIR = bin
targetinstall.path = /usr/local/bin/MuonExperiments
targetinstall.files = bin/MuonExperiments

INSTALLS += targetinstall

icon.path = /usr/local/MuonExperiments
icon.files = data/iconMuonExperiments.png
icon.extra = cp data/MuonExperiments.desktop ~/Desktop/MuonExperiments.desktop

INSTALLS += icon


OBJECTS_DIR = build/.obj
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
UI_DIR = build/.ui

TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/qcustomplot.cpp

INCLUDEPATH += include

HEADERS  += include/mainwindow.h \
    include/qcustomplot.h

FORMS    += ui/mainwindow.ui

RESOURCES += data/resources.qrc

QMAKE_CXXFLAGS += -std=c++0x

QMAKE_CXXFLAGS += -DLINUX

LIBS += -lCAENVME
