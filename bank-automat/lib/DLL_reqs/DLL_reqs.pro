QT -= gui

TEMPLATE = lib
DEFINES += DLL_REQS_LIBRARY

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dll_reqs.cpp

HEADERS += \
    DLL_reqs_global.h \
    dll_reqs.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/../../../curl/lib/ -lcurl

INCLUDEPATH += $$PWD/../../../curl/include/curl
DEPENDPATH += $$PWD/../../../curl/include/curl
