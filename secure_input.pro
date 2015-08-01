TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    siostream.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    siostream.h

