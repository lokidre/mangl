# Automatically Generated *** DO NOT EDIT
# Generated Date: 2024-10-05

# Source Group: Mangl Config Processor
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cmdline.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mangl_config_processor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mangl_config_processor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/params.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/types.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files\\Mangl Config Processor" FILES ${src})
list(APPEND main_SRC ${src})

