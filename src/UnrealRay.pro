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
        mainwindow.cpp \
    UnrealRay/Camera.cpp \
    UnrealRay/Light.cpp \
    UnrealRay/main.cpp \
    UnrealRay/Primitive.cpp \
    UnrealRay/Sampler.cpp \
    UnrealRay/Scene.cpp \
    UnrealRay/UnrealRender.cpp

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
    Normal.h \
    Transform.h \
    UnrealRay/Camera.h \
    UnrealRay/Film.h \
    UnrealRay/Light.h \
    UnrealRay/math.h \
    UnrealRay/matrix.h \
    UnrealRay/point.h \
    UnrealRay/Primitive.h \
    UnrealRay/ray.h \
    UnrealRay/Sampler.h \
    UnrealRay/Scene.h \
    UnrealRay/UnrealRender.h \
    UnrealRay/vector.h \
    Differentialgeometry.h \
    Utility.h \
    Filter.h \
    Spectrum.h \
    Integrator.h \
    Bsdf.h \
    Interaction.h

FORMS    += mainwindow.ui

DISTFILES += \
    UnrealRay/UnrealRay.vcxproj \
    UnrealRay/UnrealRay.vcxproj.filters \
    UnrealRay.sln
