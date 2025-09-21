cmake_minimum_required(VERSION 3.16)

project(sample_layout)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(layout layout.cpp)


#cmake_minimum_required(VERSION 3.16)
#
#project(sample_layout)
#
#include ( .build_config.cmake )
##include ( ${CONFIG_SCRIPTS_DIR}/project_header.cmake )
#include (${CONFIG_PROJTOOLS_DIR}/cmake/project_header.cmake)
#
#add_compile_definitions ( MANGL_NO_CONFIG_H=1 )
#
#
#include ( Sources.cmake )
#include (../../common/project_target.cmake)

#include ( ${CONFIG_SCRIPTS_DIR}/project_target.cmake )
