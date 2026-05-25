QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Model.cpp \
    drawwindow.cpp \
    loadfilewindow.cpp \
    main.cpp \
    mainwindow.cpp \
    newfilewindow.cpp \
    spriteeditorwindow.cpp \
    startupwindow.cpp

HEADERS += \
    Model.h \
    drawwindow.h \
    loadfilewindow.h \
    mainwindow.h \
    newfilewindow.h \
    spriteeditorwindow.h \
    startupwindow.h

FORMS += \
    drawwindow.ui \
    loadfilewindow.ui \
    mainwindow.ui \
    newfilewindow.ui \
    spriteeditorwindow.ui \
    startupwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
