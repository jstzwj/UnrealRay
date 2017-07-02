#-------------------------------------------------
#
# Project created by QtCreator 2017-07-02T16:52:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UnrealRay
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    Camera.h \
    Light.h \
    Point.h \
    Primitive.h \
    Ray.h \
    Sampler.h \
    Scene.h \
    Vector.h \
    Film.h \
    Matrix.h \
    Shape.h \
    Normal.h

FORMS    += mainwindow.ui
