cmake_minimum_required(VERSION 3.16)

project(mangl-font-render)

set(CONFIG_TARGET_CONSOLE ON)

include(../../.build-config.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)
add_compile_definitions(MANGL_NO_MAIN=1)
add_compile_definitions(MANGL_NO_GUI=1)



#enable_libpng()

find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIRS})
link_libraries(${FREETYPE_LIBRARIES})

include(sources.cmake)

include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources_console.cmake)

include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)

install(TARGETS ${CMAKE_PROJECT_NAME} RUNTIME DESTINATION bin)
