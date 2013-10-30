#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T15:27:03
#
#-------------------------------------------------

QT       += core gui sql

TARGET = TAEvalServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    course.cpp \
    task.cpp \
    nonadminuser.cpp \
    Storage/Storage.cpp \
    instructor.cpp \
    taeval.cpp

HEADERS  += mainwindow.h \
    course.h \
    task.h \
    nonadminuser.h \
    Storage/Storage.h \
    instructor.h \
    taeval.h

FORMS    += mainwindow.ui
