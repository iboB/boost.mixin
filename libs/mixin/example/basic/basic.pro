TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../../../

CONFIG(debug, debug|relese)
{
    LIBS += -L../../bin/qtc-mixin-debug -lmixin
}

CONFIG(release, debug|relese)
{
    LIBS += -L../../bin/qtc-mixin-release -lmixin
}

SOURCES += main.cpp

