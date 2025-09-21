# Source Group: WorkshopScreen
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
set(src
  ${CMAKE_CURRENT_LIST_DIR}/src/app.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/app.h
  ${CMAKE_CURRENT_LIST_DIR}/src/screen.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/screen.h
  ${CMAKE_CURRENT_LIST_DIR}/src/screen_animation.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/screen_call.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/screen_model.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/screen_model.h
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX "Sources\\WorkshopScreen" FILES ${src})
list(APPEND main_SRC ${src})

