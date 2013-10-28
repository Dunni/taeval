QT += core gui
QT += network

TARGET = ClientSide

TEMPLATE = app

HEADERS  += \ client.h \
    instructorscreen.h

SOURCES += main.cpp \
           client.cpp \
    instructorscreen.cpp

FORMS    += client.ui \
    instructorscreen.ui
