# Source Group: Animator
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
set(src
  ${CMAKE_CURRENT_LIST_DIR}/src/animation_info_widget.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/animation_info_widget.h
  ${CMAKE_CURRENT_LIST_DIR}/src/animation_screen.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/animation_screen.h
  ${CMAKE_CURRENT_LIST_DIR}/src/animation_widget.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/animation_widget.h
  ${CMAKE_CURRENT_LIST_DIR}/src/animator.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/animator.h
  ${CMAKE_CURRENT_LIST_DIR}/src/animator_mode.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/animator_mode.h
  ${CMAKE_CURRENT_LIST_DIR}/src/animator_types.h
  ${CMAKE_CURRENT_LIST_DIR}/src/app.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/app.h
  ${CMAKE_CURRENT_LIST_DIR}/src/gui.h
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main.h
  ${CMAKE_CURRENT_LIST_DIR}/src/main_window.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main_window.h
  ${CMAKE_CURRENT_LIST_DIR}/src/main_window_actions.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/main_window_menu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/mangl_config.h
  ${CMAKE_CURRENT_LIST_DIR}/src/res.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/res.h
  ${CMAKE_CURRENT_LIST_DIR}/src/select_widget.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/select_widget.h
  ${CMAKE_CURRENT_LIST_DIR}/src/stats_display.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/stats_display.h
  ${CMAKE_CURRENT_LIST_DIR}/src/storage.h
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Sources\\Animator" FILES ${src})
list(APPEND main_SRC ${src})

# Source Group: Resources
include_directories(${CMAKE_CURRENT_LIST_DIR}/res)
set(src
  ${CMAKE_CURRENT_LIST_DIR}/res/animator.rc
  ${CMAKE_CURRENT_LIST_DIR}/res/animator.qrc
  ${CMAKE_CURRENT_LIST_DIR}/res/resource.h
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/res PREFIX "Resources\\Resources" FILES ${src})
list(APPEND main_SRC ${src})

