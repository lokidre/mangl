cmake_minimum_required(VERSION 3.16)

project(mangl-animator)

include(.build_config.cmake)
include(${CONFIG_PROJTOOLS_DIR}/cmake/project_header.cmake)

enable_qt6()
enable_libpng()


#
# Temporary. OpenAL for windows test
#
#add_definitions ( -DMANGL_AUDIO_OPENAL=1 )
#set ( openAlDir "/local/OpenAL11" )
#include_directories ( ${openALDir} + "/include" )
#link_directories ( ${openALDir} + "/libs/Win64" )


include(Sources.cmake)

include(${CONFIG_SCRIPTS_DIR}/project_target.cmake)
