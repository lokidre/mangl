cmake_minimum_required(VERSION 3.16)

project(sample_texture)

include(../../common/tests.cmake)

add_compile_definitions(MANGL_NO_CONFIG_H=1)

addTest(sample_texture texture.cpp)


set(bundle_SRC ${CMAKE_CURRENT_LIST_DIR}/texture.yaml ${CMAKE_CURRENT_LIST_DIR}/texture.png)

set(BUNDLE_DIR "${CMAKE_BINARY_DIR}/bundle")
file(MAKE_DIRECTORY "${BUNDLE_DIR}")
foreach (file ${bundle_SRC})
    get_filename_component(targetFile ${file} NAME)
    file(CREATE_LINK "${file}" "${BUNDLE_DIR}/${targetFile}" SYMBOLIC)
endforeach()

