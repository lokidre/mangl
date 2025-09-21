cmake_minimum_required(VERSION 3.16)

cmake_minimum_required(VERSION 3.16)

project(screen_basic)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)
add_compile_definitions(MANGL_NO_MAIN=1)
add_compile_definitions(MANGL_CONSOLE=1)
set(CONFIG_TARGET_CONSOLE 1)


addTest(object screen_basic.cpp)

#target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE ${CONFIG_COLUMNS_SRC_DIR}/pchheaders.h)
