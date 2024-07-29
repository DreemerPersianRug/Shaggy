QT       += core gui printsupport opengl

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
    complexobjects.cpp \
    eulerangles.cpp \
    geometric2dprimitives.cpp \
    geometric3dprimitives.cpp \
    glwidget.cpp \
    inertiaestimator.cpp \
    labelupdate.cpp \
    mahonyfilter.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    quaternion.cpp \
    sensordatareceiver.cpp \
    transformvalues.cpp \


HEADERS += \
    complexobjects.h \
    eulerangles.h \
    geometric2dprimitives.h \
    geometric3dprimitives.h \
    glwidget.h \
    inertiaestimator.h \
    labelupdate.h \
    mahonyfilter.h \
    mainwindow.h \
    qcustomplot.h \
    quaternion.h \
    sensordatareceiver.h \
    transformvalues.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#INCLUDEPATH += /path/to/glew/include
INCLUDEPATH += /path/to/glu/include $$PWD/eigen

#LIBS += -L/path/to/glew/lib
LIBS += -L/path/to/glu/lib

#LIBS += -lGLEW
LIBS += -lGLU
