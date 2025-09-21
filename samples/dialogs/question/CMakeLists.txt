cmake_minimum_required(VERSION 3.16)

project(mangl_question_dialog)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(mangl_question_dialog question_dialog.cpp)
