cmake_minimum_required(VERSION 3.16)

project(sample_line)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

add_executable ( vector_format vector_format.cpp )

#include ( Sources.cmake )
#include ( ${root_DIR}/tools/cmake/project.cmake )

