cmake_minimum_required(VERSION 3.20)

project(sample_circle)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(sample_circle sample_circle.cpp)

