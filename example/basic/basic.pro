TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../include

CONFIG(debug, debug|relese)
{
    LIBS += -L../../bin/qtc-mixin-debug -lmixin
}

CONFIG(release, debug|relese)
{
    LIBS += -L../../bin/qtc-mixin-release -lmixin
}

HEADERS += d3d_renderer.hpp        object_manager.hpp      transform_messages.hpp \
           d3d_renderer_mixin.hpp  rendering_messages.hpp  transform_mixin.hpp \
           gl_renderer.hpp         system_messages.hpp     basic.hpp \
           gl_renderer_mixin.hpp   transform.hpp           has_id_mixin.hpp \
           has_id.hpp


SOURCES += d3d_renderer.cpp  object_manager.cpp      transform.cpp \
           gl_renderer.cpp   rendering_messages.cpp  transform_messages.cpp \
           main.cpp          system_messages.cpp     has_id.cpp


