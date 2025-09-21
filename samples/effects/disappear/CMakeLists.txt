cmake_minimum_required(VERSION 3.16)

project(sample_effects_disappear)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(sample_effects_disappear sample_disappear.cpp)
