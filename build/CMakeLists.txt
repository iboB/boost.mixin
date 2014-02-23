cmake_minimum_required(VERSION 2.8.6)

project(boost_mixin)

# compiler specific stuff
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC"))
    # nothing yet
else() # assuming gcc compatible compiler
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++0x")
endif()


set(BM_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
set(BM_SRC_DIR ${BM_ROOT}/libs/mixin/src)

add_library(boost_mixin
    SHARED
    ${BM_SRC_DIR}/common_mutation_rules.cpp
    ${BM_SRC_DIR}/mixin_collection.cpp
    ${BM_SRC_DIR}/object_type_mutation.cpp
    ${BM_SRC_DIR}/domain.cpp
    ${BM_SRC_DIR}/allocators.cpp
    ${BM_SRC_DIR}/object.cpp
    ${BM_SRC_DIR}/object_type_template.cpp
    ${BM_SRC_DIR}/global.cpp
    ${BM_SRC_DIR}/object_mutator.cpp
    ${BM_SRC_DIR}/same_type_mutator.cpp
    ${BM_SRC_DIR}/object_type_info.cpp
    ${BM_SRC_DIR}/single_object_mutator.cpp
)

# cached to use in other projects
set(BM_INCLUDE_DIR
    ${BM_ROOT}
    CACHE STRING "boost.mixin include dir"
)

include_directories(
    # ??? ${BOOST_INCLUDE_DIRS}
    ${BOOST_MIXIN_INCLUDE_DIRS}
)

# add_definitions( .... )

# output stuff for other projects
# all cached to use in other projects

set(BM_LINK_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    CACHE STRING "boost.mixin link path"
)

set(BM_LIB_NAME
    boost_mixin
    CACHE STRING "boost.mixin lib name"
)
