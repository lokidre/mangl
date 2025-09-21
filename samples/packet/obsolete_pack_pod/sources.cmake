# Source Group: Main
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/pod.cpp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Sources\\Main" FILES ${src})
list(APPEND main_SRC ${src})

