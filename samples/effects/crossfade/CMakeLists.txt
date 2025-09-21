cmake_minimum_required(VERSION 3.16)

project(sample_effects_crossfade)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(sample_effects_crossfade sample_crossfade.cpp)
