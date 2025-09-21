cmake_minimum_required(VERSION 3.22)

project(mangl-make-tex)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type (e.g. Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()


set(CONFIG_PROJECT_CONSOLE ON)
add_compile_definitions(MANGL_NO_CONFIG_H=1)

include(../../.build-config.cmake)

include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)

include(sources.cmake)

include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources_console.cmake)

include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)


install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)

