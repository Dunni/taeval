QT       += core gui
QT       += network


TARGET = TAEvalClient
TEMPLATE = app


SOURCES += main.cpp\
        mainscreen.cpp \
    client.cpp \
    ../Common/course.cpp \
    ../Common/nonadminuser.cpp \
    ../Common/ta.cpp \
    ../Common/task.cpp

HEADERS  += mainscreen.h \
    client.h \
    ../Common/course.h \
    ../Common/nonadminuser.h \
    ../Common/ta.h \
    ../Common/task.h \
    client.h

FORMS    += mainscreen.ui

RESOURCES += \
    pictures.qrc

OTHER_FILES +=
