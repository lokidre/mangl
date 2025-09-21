cmake_minimum_required(VERSION 3.16)

project(sample_rect)

include(../../../.build-config.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

include_directories(${CONFIG_MANGL_DIR}/samples)
list(APPEND main_SRC rect.cpp)

include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources.cmake)

include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)
