#-------------------------------------------------
#
# Project created by QtCreator 2017-11-01T21:22:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pmp-gui
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        pmpgui.cpp

HEADERS  += pmpgui.h

FORMS    += pmpgui.ui

VERSION = 0.2.0

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../player/release/ -lplayer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../player/debug/ -lplayer
else:unix: LIBS += -L$$OUT_PWD/../player/ -lplayer

INCLUDEPATH += $$PWD/../player
DEPENDPATH += $$PWD/../player

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../playList/release/ -lplayList
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../playList/debug/ -lplayList
else:unix: LIBS += -L$$OUT_PWD/../playList/ -lplayList

INCLUDEPATH += $$PWD/../playList
DEPENDPATH += $$PWD/../playList
