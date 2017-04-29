#-------------------------------------------------
#
# Project created by QtCreator 2016-09-02T05:54:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EmproDatalogViewer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Widgets/Datalog/dialogplotchannelchoose.cpp \
    Widgets/Datalog/plotdatalog.cpp \
    Widgets/Datalog/pointlist.cpp \
    Widgets/Datalog/widgetdatalogviewcontrol.cpp \
    helpers.cpp \
    Widgets/Datalog/horizontalaxis.cpp \
    Widgets/Datalog/verticalaxis.cpp

HEADERS  += mainwindow.h \
    Widgets/Datalog/dialogplotchannelchoose.h \
    Widgets/Datalog/plotdatalog.h \
    Widgets/Datalog/pointlist.h \
    Widgets/Datalog/widgetdatalogviewcontrol.h \
    helpers.h \
    Widgets/Datalog/horizontalaxis.h \
    Widgets/Datalog/verticalaxis.h

FORMS    += mainwindow.ui \
    Widgets/Datalog/dialogplotchannelchoose.ui

DISTFILES += \
    Readme.txt
