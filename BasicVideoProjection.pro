QT       += core gui
QT       += multimedia
QT       += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += \
    $$PWD/QtAwesome

include(QtAwesome/QtAwesome.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    controlwindow.cpp \
    playerpositiondialog.cpp \
    playerwindow.cpp \
    qrelurl.cpp \
    scene.cpp \
    videowidget.cpp

HEADERS += \
    controlwindow.h \
    playerpositiondialog.h \
    playerwindow.h \
    qrelurl.h \
    scene.h \
    videowidget.h

FORMS += \
    controlwindow.ui \
    playerpositiondialog.ui \
    playerwindow.ui

TRANSLATIONS += \
    BasicVideoProjection_de_DE.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
