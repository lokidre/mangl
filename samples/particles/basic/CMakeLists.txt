cmake_minimum_required(VERSION 3.16)

project(particles_basic)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(particles particles_basic.cpp)



#cmake_minimum_required(VERSION 3.10)
#
#project(particles_basic)
#
#set ( root_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../.. )
#
#include ( ${root_DIR}/tools/cmake/setup.cmake )
#
#include ( Sources.cmake )
#
#include ( ${root_DIR}/tools/cmake/project.cmake )

