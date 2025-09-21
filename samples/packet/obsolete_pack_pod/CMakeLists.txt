cmake_minimum_required(VERSION 3.16)

project(packet_pod)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)
add_compile_definitions(MANGL_NO_MAIN=1)
add_compile_definitions(MANGL_CONSOLE=1)

set(CONFIG_TARGET_CONSOLE 1)

addTest(pod src/pod.cpp)



#cmake_minimum_required(VERSION 3.16)
#
#project(packet_pod)
#
#include(.build_project_config.cmake)
#include(${CONFIG_MANGL_DIR}/scripts/cmake/project_header.cmake)
#
#add_compile_definitions(MANGL_NO_CONFIG_H=1)
#
#include(sources.cmake)
#
#include(${CONFIG_MANGL_DIR}/scripts/cmake/project_target.cmake)
