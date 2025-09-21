cmake_minimum_required(VERSION 3.16)

cmake_minimum_required(VERSION 3.16)

project(storage_settings)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)
add_compile_definitions(MANGL_NO_MAIN=1)
add_compile_definitions(MANGL_CONSOLE=1)
set(CONFIG_TARGET_CONSOLE 1)


addTest(object storage_settings.cpp)

#add_compile_definitions ( MANGL_CONSOLE=1 )


#include ( Sources.cmake )

#include ( ${CONFIG_SCRIPTS_DIR}/project_target.cmake )
