QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle



SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qtsocks4lib/release/ -lqtsocks4lib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qtsocks4lib/debug/ -lqtsocks4lib
else:unix: LIBS += -L$$OUT_PWD/../qtsocks4lib/ -lqtsocks4lib

INCLUDEPATH += $$PWD/../qtsocks4lib
DEPENDPATH += $$PWD/../qtsocks4lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtsocks4lib/release/libqtsocks4lib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtsocks4lib/debug/libqtsocks4lib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtsocks4lib/release/qtsocks4lib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qtsocks4lib/debug/qtsocks4lib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qtsocks4lib/libqtsocks4lib.a
