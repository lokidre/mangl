cmake_minimum_required(VERSION 3.16)

project(mangl-make-ico)

include(.build_config.cmake)
include(${CONFIG_PROJTOOLS_DIR}/cmake/project_header.cmake)

set(CONFIG_TARGET_CONSOLE ON)

enable_libpng()

include(Sources.cmake)

include(${CONFIG_SCRIPTS_DIR}/project_target.cmake)

install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION BIN)
