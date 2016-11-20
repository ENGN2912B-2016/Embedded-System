#-------------------------------------------------
#
# Project created by QtCreator 2016-11-20T04:09:15
#
#-------------------------------------------------

QT += multimedia

TARGET = audio
TEMPLATE = app

SOURCES += main.cpp\
        widget.cpp \
    IOdevice.cpp \


HEADERS  += widget.h \
    IOdevice.h \




INCLUDEPATH += ../../../include

LIBS += -L$$OUT_PWD/../../../lib

TEMPLATE = app

QT += charts
QT += core gui widgets

contains(TARGET, qml.*) {
    QT += qml quick
}

target.path = $$[QT_INSTALL_EXAMPLES]/charts/$$TARGET
INSTALLS += target

