cmake_minimum_required(VERSION 3.16)

project(mangl-animator)

include(../../.build-config.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)

enable_qt6()
# enable_libpng()

include(sources.cmake)

include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources.cmake)

include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)
