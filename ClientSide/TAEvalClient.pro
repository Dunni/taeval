QT       += core gui
QT       += network


TARGET = TAEvalClient
TEMPLATE = app

DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS  += \
    ../Common/course.h \
    ../Common/nonadminuser.h \
    ../Common/ta.h \
    ../Common/task.h \
    ../Common/clientconnection.h \
    ../Common/Communication/clientconnection.h \
    Client/taapplogic.h \
    Client/mainapplogic.h \
    Client/instructorapplogic.h \
    Client/ui_mainscreen.h

SOURCES += main.cpp\
    ../Common/course.cpp \
    ../Common/nonadminuser.cpp \
    ../Common/ta.cpp \
    ../Common/task.cpp \
    ../Common/Communication/clientconnection.cpp \
    Client/taapplogic.cpp \
    Client/mainapplogic.cpp \
    Client/instructorapplogic.cpp



FORMS += Client/mainscreen.ui

RESOURCES += \
    Client/pictures.qrc

OTHER_FILES += \
    ../Common/Communication/config.ini
