cmake_minimum_required(VERSION 3.20)

project(mangl-toolkit)

set(CONFIG_MANGL_NO_MAIN true)
set(CONFIG_MANGL_GTK TRUE)

include(../../.build-config.cmake)

include(${CONFIG_MANGL_CMAKE_DIR}/project_header.cmake)

if (WIN32)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()


set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(GEN_DIR ${SRC_DIR}/generated)
set(RES_DIR ${SRC_DIR}/res)

#set(RES_FILE ${RES_DIR}/resources.xml)

#set(RES_DEPS ${RES_FILE} ${RES_DIR}/theme.css ${RES_DIR}/main-menu.xml)


#if (UNIX)
#add_custom_command(
#        OUTPUT ${GEN_DIR}/resources.c
#        COMMAND glib-compile-resources --target=${GEN_DIR}/resources.c --generate-source ${RES_FILE}
#        WORKING_DIRECTORY ${RES_DIR}
#        DEPENDS ${RES_DEPS}
#        VERBATIM
#)
#
#add_custom_command(
#        OUTPUT ${GEN_DIR}/resources.h
#        COMMAND glib-compile-resources --target=${GEN_DIR}/resources.h --generate-header ${RES_FILE}
#        WORKING_DIRECTORY ${RES_DIR}
#        DEPENDS ${RES_DEPS}
#        VERBATIM
#)
#endif()

include(sources.cmake)

include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)
include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources.cmake)

include(${CONFIG_MANGL_CMAKE_DIR}/project_target.cmake)
