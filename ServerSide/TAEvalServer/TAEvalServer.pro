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
    ../../Common/task.cpp \
    ../../Common/nonadminuser.cpp \
    Storage/Storage.cpp \
    ../../Common/instructor.cpp \
    taeval.cpp \
    ../../Common/ta.cpp \
   ../../Common/course.cpp

HEADERS  += mainwindow.h \
    ../../Common/course.h \
    ../../Common/task.h \
    ../../Common/nonadminuser.h \
    Storage/Storage.h \
    ../../Common/instructor.h \
    taeval.h \
    ../../Common/ta.h

FORMS    += mainwindow.ui
