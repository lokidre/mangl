# Source Group: Packet Dump
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
set(src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/packet_dump.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/packet_dump.h
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Sources\\Packet Dump" FILES ${src})
list(APPEND main_SRC ${src})

