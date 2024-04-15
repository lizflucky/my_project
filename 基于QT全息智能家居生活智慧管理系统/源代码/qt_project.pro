QT       += core gui
QT += core
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimediawidgets
QT += multimedia

CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    choice_model.cpp \
    main.cpp \
    mainwindow.cpp \
    music.cpp \
    smart_furniture.cpp \
    weather.cpp

HEADERS += \
    camera.h \
    choice_model.h \
    mainwindow.h \
    music.h \
    smart_furniture.h \
    weather.h

FORMS += \
    camera.ui \
    choice_model.ui \
    mainwindow.ui \
    music.ui \
    smart_furniture.ui \
    weather.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    air.qrc \
    furniture.qrc \
    main.qrc \
    model.qrc \
    res.qrc \
    weather.qrc \
    weather2.qrc \
    weather_picture.qrc
