# Automatically Generated *** DO NOT EDIT
# Generated Date: 2024-09-27

# Source Group: Toolkit
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
set(src
  ${CMAKE_CURRENT_LIST_DIR}/src/central_widget.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/central_widget.h
  ${CMAKE_CURRENT_LIST_DIR}/src/env.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/generated/main_window_layout_state.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/generated/model_view_state.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/generated/project_files_state.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/generated/screen_layout_state.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main_application.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main_application.h
  ${CMAKE_CURRENT_LIST_DIR}/src/main_data.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main_data.h
  ${CMAKE_CURRENT_LIST_DIR}/src/main_types.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main_window.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main_window.h
  ${CMAKE_CURRENT_LIST_DIR}/src/mangl_config.h
  ${CMAKE_CURRENT_LIST_DIR}/src/pubsub.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/settings.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/state.hpp
  ${CMAKE_CURRENT_LIST_DIR}/src/theme.hpp
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Sources\\Toolkit" FILES ${src})
list(APPEND main_SRC ${src})

