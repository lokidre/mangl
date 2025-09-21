cmake_minimum_required(VERSION 3.20)

project(mangl-toolkit)

include(../../.build-config.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)

set(MANGL_NO_MAIN true)
add_compile_definitions(MANGL_NO_MAIN=1)


enable_wxwidgets()
#enable_libpng()


#find_package(wxWidgets REQUIRED COMPONENTS net core base)
#if(wxWidgets_USE_FILE) # not defined in CONFIG mode
#    include(${wxWidgets_USE_FILE})
#endif()
#include_directories(${wxWidgets_INCLUDE_DIRS})
#link_directories(${wxWidgets_LIBRARY_DIRS})
#link_libraries(${wxWidgets_LIBRARIES})


set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(GEN_DIR ${SRC_DIR}/generated)
set(RES_DIR ${SRC_DIR}/res)

include(sources.cmake)

include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources.cmake)

include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)
