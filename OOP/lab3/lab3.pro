QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
    builder/basebuilder.cpp \
    builder/builderimp.cpp \
    builder/director.cpp \
    command/basecommand.cpp \
    command/transformcommand.cpp \
    drawer/basedrawer.cpp \
    drawer/drawer.cpp \
    drawer/drawerfactory.cpp \
    drawer/visualizer.cpp \
    main.cpp \
    mainwindow.cpp \
    manager/basescenemanager.cpp \
    manager/controlmanager.cpp \
    manager/transformmanager.cpp \
    object/basecamera.cpp \
    object/baseobject.cpp \
    object/camera.cpp \
    object/compositeobject.cpp \
    object/model.cpp \
    object/point.cpp \
    object/simpleobjects.cpp \
    scene/scene.cpp \
    scene/sceneinterface.cpp \
    visitor/transformvisitor.cpp

HEADERS += \
    builder/basebuilder.h \
    builder/builderimp.h \
    builder/director.h \
    command/basecommand.h \
    command/transformcommand.h \
    drawer/basedrawer.h \
    drawer/drawer.h \
    drawer/drawerfactory.h \
    drawer/visualizer.h \
    error.h \
    iterator.h \
    mainwindow.h \
    manager/basescenemanager.h \
    manager/controlmanager.h \
    manager/transformmanager.h \
    object/basecamera.h \
    object/baseobject.h \
    object/camera.h \
    object/compositeobject.h \
    object/model.h \
    object/point.h \
    object/simpleobjects.h \
    scene/basescene.h \
    scene/scene.h \
    scene/sceneinterface.h \
    visitor/objectvisitor.h \
    visitor/transformvisitor.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    data/cube.txt \
    data/test.txt \
    lab3.pro.user
