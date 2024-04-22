QT += core gui
QT += serialport
QT += network
QT += core


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/lib/DLL_rfid/build/Desktop_x86_windows_msys_pe_64bit-Debug/debug/ -lDLL_rfid
win32: LIBS += -L$$PWD/../../death/lib -lcurl

INCLUDEPATH += $$PWD/lib/DLL_rfid
DEPENDPATH += $$PWD/lib/DLL_rfid
INCLUDEPATH += $$PWD/../../death/include
DEPENDPATH += $$PWD/../../death/include

win32: LIBS += -L$$PWD/lib/DLL_reqs/build/debug/ -lreqs

INCLUDEPATH += $$PWD/lib/DLL_reqs
DEPENDPATH += $$PWD/lib/DLL_reqs
