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
    task.cpp \
    nonadminuser.cpp \
    Storage/Storage.cpp \
    instructor.cpp \
    taeval.cpp \
    ta.cpp \
    ../../Common/task.cpp \
    ../../Common/ta.cpp \
    ../../Common/nonadminuser.cpp \
    ../../Common/instructor.cpp \
    ../../Common/course.cpp

HEADERS  += mainwindow.h \
    course.h \
    task.h \
    nonadminuser.h \
    Storage/Storage.h \
    instructor.h \
    taeval.h \
    ta.h \
    ../../Common/task.h \
    ../../Common/ta.h \
    ../../Common/nonadminuser.h \
    ../../Common/instructor.h \
    ../../Common/course.h

FORMS    += mainwindow.ui
