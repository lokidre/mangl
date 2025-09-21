cmake_minimum_required(VERSION 3.22)

project(mp_sample_client)

set(CONFIG_PROJECT_MULTIPLAYER ON)
set(CONFIG_PROJECT_CONSOLE ON)
set(CONFIG_PROJECT_NO_CONFIG_H ON)

include(${CMAKE_CURRENT_LIST_DIR}/../../../.build-config.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../shared)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../common/src)


addMainSrc(
    ${CMAKE_CURRENT_LIST_DIR}/sample_client.cpp
    ${CMAKE_CURRENT_LIST_DIR}/sample_client_packet.cpp
    ${CMAKE_CURRENT_LIST_DIR}/sample_client_packet.h
)

include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources_console.cmake)


include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)

