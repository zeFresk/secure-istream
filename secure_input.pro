TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++14

SOURCES += main.cpp \
    sistream.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    sistream.h

