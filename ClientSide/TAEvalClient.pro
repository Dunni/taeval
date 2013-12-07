QT       += core gui
QT       += network


TARGET = TAEvalClient
TEMPLATE = app

#DEFINES     += QT_NO_DEBUG_OUTPUT

HEADERS  += \
#    client.h \
    ../Common/course.h \
    ../Common/nonadminuser.h \
    ../Common/ta.h \
    ../Common/task.h \
#    ../Common/connection.h \
    ../Common/clientconnection.h \
    instructorapplogic.h \
    taapplogic.h \
    mainapplogic.h
#    ../Common/serverconnection.h
#    client.h

SOURCES += main.cpp\
#    client.cpp \
    ../Common/course.cpp \
    ../Common/nonadminuser.cpp \
    ../Common/ta.cpp \
#    ../Common/connection.cpp \
    ../Common/clientconnection.cpp \
#    ../Common/serverconnection.cpp \
    ../Common/task.cpp \
    instructorapplogic.cpp \
    taapplogic.cpp \
    mainapplogic.cpp



FORMS    += mainscreen.ui

RESOURCES += \
    pictures.qrc

OTHER_FILES +=
