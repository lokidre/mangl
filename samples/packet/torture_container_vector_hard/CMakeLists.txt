cmake_minimum_required(VERSION 3.16)

project(torture_container_vector_hard_packet_test)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)
add_compile_definitions(MANGL_NO_MAIN=1)
add_compile_definitions(MANGL_CONSOLE=1)

set(CONFIG_TARGET_CONSOLE 1)

addTest(torture_container_vector_hard container.cpp)

#
#cmake_minimum_required(VERSION 3.16)
#
#project(binary_packet_test)
#
#include(.build_project_config.cmake)
##include(${CONFIG_SCRIPTS_DIR}/project_header.cmake)
#include(${CONFIG_PROJTOOLS_DIR}/cmake/project_header.cmake)
#
#add_compile_definitions(MANGL_NO_CONFIG_H=1)
#
#add_compile_definitions(MANGL_NO_MAIN=1)
#add_compile_definitions(MANGL_CONSOLE=1)
#
#set(CONFIG_TARGET_CONSOLE 1)
#
#enable_libpng()
#
#include(sources.cmake)
#
#include(../../common/project_target.cmake)
#
##include(${CONFIG_SCRIPTS_DIR}/project_target.cmake)
