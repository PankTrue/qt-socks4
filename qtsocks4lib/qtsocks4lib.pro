QT       -= gui
QT       += network

TARGET = qtsocks4lib
TEMPLATE = lib
CONFIG += staticlib


SOURCES += \
    socks5server_impl.cpp \
    socks4client_impl.cpp \
    qt-socks4.cpp

HEADERS += \
    socks5server_impl.h \
    socks4client_impl.h \
    qt-socks4.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
