#-------------------------------------------------
#
# Project created by QtCreator 2017-07-05T20:04:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UnrealRay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    core/Film.h \
    core/Integrator.h \
    core/Light.h \
    core/Matrix.h \
    core/Normal.h \
    core/Point.h \
    core/Primitive.h \
    core/Ray.h \
    core/Sampler.h \
    core/Scene.h \
    core/Shape.h \
    core/Spectrum.h \
    core/Transform.h \
    core/Utility.h \
    core/Vector.h \
    driver/unreal.h \
    core/Integrator.h \
    core/Scene.h \
    core/Spectrum.h \
    core/Matrix.h \
    core/Normal.h \
    core/Point.h \
    core/Transform.h \
    core/Utility.h \
    core/Vector.h \
    core/Film.h \
    core/Light.h \
    core/Primitive.h \
    core/Ray.h \
    core/Sampler.h \
    core/Shape.h \
    core/Bounds.h \
    core/Type.h

FORMS    += mainwindow.ui

DISTFILES += \
    build-UnrealRay-Desktop_Qt_5_7_0_MinGW_32bit-Debug/.qmake.stash
