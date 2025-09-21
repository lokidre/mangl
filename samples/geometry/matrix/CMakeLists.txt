cmake_minimum_required(VERSION 3.20)

project(sample_matrix)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)
add_compile_definitions(MANGL_NO_MAIN=1)
add_compile_definitions(MANGL_CONSOLE=1)

set(CONFIG_TARGET_CONSOLE 1)

addTest(sample_matrix matrix_format.cpp)

#include ( Sources.cmake )
#include ( ${root_DIR}/tools/cmake/project.cmake )

