QT       += core gui
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

TARGET = robosim
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scenewidget.cpp \
    scene.cpp \
    routingalgorithm.cpp \
    dummyalgorithm.cpp \
    routingalgorithmregistry.cpp \
    simulation.cpp \
    robot.cpp \
    potentialfieldalgorithm.cpp \
    qcustomplot.cpp \
    plot.cpp \
    signaltoplot.cpp

HEADERS  += mainwindow.h \
    scenewidget.h \
    scene.h \
    routingalgorithm.h \
    dummyalgorithm.h \
    routingalgorithmregistry.h \
    simulation.h \
    array2d.h \
    robot.h \
    potentialfieldalgorithm.h \
    qcustomplot.h \
    plot.h \
    signaltoplot.h

FORMS    += mainwindow.ui \
    plot.ui
