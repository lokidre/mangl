cmake_minimum_required(VERSION 3.16)

project(effects_chain)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(effects chain.cpp)