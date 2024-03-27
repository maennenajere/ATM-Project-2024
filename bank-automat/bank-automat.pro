QT       += core gui
QT += serialport
QT += serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    actionmenu.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    actionmenu.h \
    login.h \
    mainwindow.h

FORMS += \
    actionmenu.ui \
    login.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/lib/DLL_rfid/build/debug/ -lDLL_rfid

INCLUDEPATH += $$PWD/lib/DLL_rfid
DEPENDPATH += $$PWD/lib/DLL_rfid
