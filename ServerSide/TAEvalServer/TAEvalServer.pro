#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T15:27:03
#
#-------------------------------------------------

QT       += core sql network

TARGET = TAEvalServer
TEMPLATE = app


SOURCES += main.cpp\
    ../../Common/task.cpp \
    ../../Common/nonadminuser.cpp \
    Storage/Storage.cpp \
    ../../Common/instructor.cpp \
    taeval.cpp \
    ../../Common/ta.cpp \
   ../../Common/course.cpp \
    server.cpp \
    accesscontrol.cpp

HEADERS  += \
    ../../Common/course.h \
    ../../Common/task.h \
    ../../Common/nonadminuser.h \
    Storage/Storage.h \
    ../../Common/instructor.h \
    taeval.h \
    ../../Common/ta.h \
    server.h \
    accesscontrol.h

FORMS    += mainwindow.ui
