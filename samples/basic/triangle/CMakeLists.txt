cmake_minimum_required(VERSION 3.16)

project(sample_triangle)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(triangle triangle.cpp)

#cmake_minimum_required(VERSION 3.16)
#
#project(sample_triangle)
#
#include(.build_config.cmake)
#
#include(${CONFIG_PROJTOOLS_DIR}/cmake/project_header.cmake)
#
#add_compile_definitions(MANGL_NO_CONFIG_H=1)
#
#include(Sources.cmake)
#
#include(../../common/project_target.cmake)
