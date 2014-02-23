QT       -= qt core gui

TARGET = mixin
TEMPLATE = lib
CONFIG = dll

QMAKE_CXXFLAGS += -std=c++0x

INCLUDEPATH += ../../../../

mixin_src = ../../src

SOURCES += \
    $${mixin_src}/common_mutation_rules.cpp \
    $${mixin_src}/domain.cpp \
    $${mixin_src}/global.cpp \
    $${mixin_src}/internal.hpp \
    $${mixin_src}/mixin_collection.cpp \
    $${mixin_src}/object.cpp \
    $${mixin_src}/object_mutator.cpp \
    $${mixin_src}/object_type_info.cpp \
    $${mixin_src}/object_type_mutation.cpp \
    $${mixin_src}/object_type_template.cpp \
    $${mixin_src}/same_type_mutator.cpp \
    $${mixin_src}/single_object_mutator.cpp \
    $${mixin_src}/allocators.cpp \

mixin_inc = ../../../../boost/mixin

HEADERS += \
    $${mixin_inc}/../mixin.hpp \
    \
    $${mixin_inc}/combinators.hpp \
    $${mixin_inc}/common_mutation_rules.hpp \
    $${mixin_inc}/config.hpp \
    $${mixin_inc}/domain.hpp \
    $${mixin_inc}/exception.hpp \
    $${mixin_inc}/feature.hpp \
    $${mixin_inc}/feature_parser.hpp \
    $${mixin_inc}/global.hpp \
    $${mixin_inc}/message.hpp \
    $${mixin_inc}/message_macros.hpp \
    $${mixin_inc}/mixin_collection.hpp \
    $${mixin_inc}/mixin.hpp \
    $${mixin_inc}/mixin_type_info.hpp \
    $${mixin_inc}/mutation_rule.hpp \
    $${mixin_inc}/object.hpp \
    $${mixin_inc}/object_mutator.hpp \
    $${mixin_inc}/object_type_info.hpp \
    $${mixin_inc}/object_type_mutation.hpp \
    $${mixin_inc}/object_type_template.hpp \
    $${mixin_inc}/same_type_mutator.hpp \
    $${mixin_inc}/single_object_mutator.hpp \
    $${mixin_inc}/allocators.hpp \
    \
    $${mixin_inc}/gen/message_macros.ipp \
    \
    $${mixin_src}/internal.hpp

