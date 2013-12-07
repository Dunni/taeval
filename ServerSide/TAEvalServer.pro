#-------------------------------------------------
#
# Project created by QtCreator 2013-10-29T15:27:03
#
#-------------------------------------------------

QT       += core sql network

TARGET = TAEvalServer
TEMPLATE = app

DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp\
    Database/PersistImpSQL.cpp \
    Server/taeval.cpp \
    Server/server.cpp \
    Server/accesscontrol.cpp \
    ../Common/Communication/serverconnection.cpp \
    ../Common/task.cpp \
    ../Common/ta.cpp \
    ../Common/nonadminuser.cpp \
    ../Common/instructor.cpp \
    ../Common/course.cpp \
    Storage/Storage.cpp

HEADERS  += \
    Storage/PersistImp.h \
    Database/PersistImpSQL.h \
    Server/taeval.h \
    Server/server.h \
    Server/accesscontrol.h \
    ../Common/Communication/serverconnection.h \
    ../Common/task.h \
    ../Common/ta.h \
    ../Common/nonadminuser.h \
    ../Common/instructor.h \
    ../Common/course.h \
    Storage/Storage.h

OTHER_FILES += \
    Database/taeval.sql \
    ../Common/Communication/config.ini

