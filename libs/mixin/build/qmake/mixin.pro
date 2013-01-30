QT       -= qt core gui

TARGET = mixin
TEMPLATE = lib
CONFIG = dll

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../../../

mixin_src = ../../src

SOURCES += \
    $${mixin_src}/domain.cpp \
    $${mixin_src}/global.cpp \
    $${mixin_src}/object_type_info.cpp \
    $${mixin_src}/object.cpp \
    $${mixin_src}/object_transformer.cpp \


mixin_inc = ../../../../boost/mixin

HEADERS += \
    $${mixin_inc}/../mixin.hpp \
    $${mixin_inc}/config.hpp \
    $${mixin_inc}/global.hpp \
    $${mixin_inc}/domain.hpp \
    $${mixin_inc}/feature.hpp \
    $${mixin_inc}/mixin_type_info.hpp \
    $${mixin_inc}/mixin.hpp \
    $${mixin_inc}/message.hpp \
    $${mixin_inc}/message_macros.hpp \
    $${mixin_inc}/feature_parser.hpp \
    $${mixin_inc}/object_type_info.hpp \
    $${mixin_inc}/object.hpp \
    $${mixin_inc}/exception.hpp \
    $${mixin_inc}/object_transformer.hpp \
    \
    $${mixin_inc}/third_party/fd_simplify_mem_func.hpp \
    \
    $${mixin_src}/internal.hpp

