cmake_minimum_required(VERSION 3.22)

project(mp_sample_server)

set(CONFIG_PROJECT_MULTIPLAYER ON)
set(CONFIG_PROJECT_UNIX_DAEMON ON)
set(CONFIG_PROJECT_CONSOLE ON)
set(CONFIG_PROJECT_NO_CONFIG_H ON)


include(${CMAKE_CURRENT_LIST_DIR}/../../../.build-config.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../shared)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../common/src)

set(src
    ${CMAKE_CURRENT_LIST_DIR}/daemonize.cpp
    ${CMAKE_CURRENT_LIST_DIR}/daemonize.h
    ${CMAKE_CURRENT_LIST_DIR}/sample_server.cpp
)
list(APPEND main_SRC ${src})

include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)
#include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources_console.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)

