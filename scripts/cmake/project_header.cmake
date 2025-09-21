include (${CMAKE_CURRENT_LIST_DIR}/project_header_inc.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/project_deps.cmake)


if (ANDROID)

#    add_compile_definitions(HAVE_ZLIB=1)
#    add_compile_definitions(HAVE_LIBPNG=1)

elseif(EMSCRIPTEN)

else()
#    enable_libpng()
endif()
