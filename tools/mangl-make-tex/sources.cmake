# Automatically Generated *** DO NOT EDIT
# Generated Date: 2024-11-13

# Source Group: Texture Maker
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/build_deps.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/build_desc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/build_header.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/build_image.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/build_placements.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/build_placements_brute_force.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/build_placements_rectpack.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/cmdline.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dfs_placements.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/image_bleed.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/image_load.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/image_scale.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/load_desc.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/load_font.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/params.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/process.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/process_range.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rectpack2D/best_bin_finder.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rectpack2D/empty_space_allocators.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rectpack2D/empty_spaces.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rectpack2D/finders_interface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rectpack2D/insert_and_split.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/rectpack2D/rect_structs.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/texmake.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/types.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files\\Texture Maker" FILES ${src})
list(APPEND main_SRC ${src})

