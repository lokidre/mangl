#
#

file(REAL_PATH "${CMAKE_CURRENT_LIST_DIR}/../.." MANGL_DIR)

include(${MANGL_DIR}/.build-config.cmake)

include (${MANGL_DIR}/scripts/cmake/project_header_inc.cmake)

set(MANGL_TESTS_DIR ${CONFIG_MANGL_DIR}/samples)

include_directories(${MANGL_TESTS_DIR}/common)
include(${MANGL_TESTS_DIR}/common/common_sources.cmake)


include(${CONFIG_APEAL_CMAKE_DIR}/apeal_sources.cmake)
include(${CONFIG_COGNIT_CMAKE_DIR}/cognit_sources.cmake)

if (CONFIG_PROJECT_CONSOLE)
    include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources_console.cmake)
else()
    include(${CONFIG_MANGL_CMAKE_DIR}/mangl_sources.cmake)
endif()

include(${MANGL_DIR}/scripts/cmake/project_deps.cmake)
include(${MANGL_DIR}/scripts/cmake/project_link.cmake)


if (bundle_SRC)
    set(BUNDLE_DIR "${CMAKE_BINARY_DIR}/bundle")
    file(MAKE_DIRECTORY "${BUNDLE_DIR}")

    foreach (file ${bundle_SRC})
        get_filename_component(targetFile ${file} NAME)
        if (WIN32)
            file(CREATE_LINK "${file}" "${BUNDLE_DIR}/${targetFile}")
        else()
            file(CREATE_LINK "${file}" "${BUNDLE_DIR}/${targetFile}" SYMBOLIC)
        endif()
    endforeach()
endif()

if (WIN32)
    if ((NOT DEFINED CONFIG_TARGET_CONSOLE) OR ($<NOT:${CONFIG_TARGET_CONSOLE}>))
        set (GUI_TYPE WIN32)
    endif()
endif()


macro (addTest targetName targetSrc)

    #set(targetSrc ${targetName}.cpp)

    if (EMSCRIPTEN)
        set(targetName wasm)
    endif()

    add_executable(${targetName} ${GUI_TYPE} ${targetSrc} ${main_SRC})

    if (EMSCRIPTEN)
        set(WASM_LINKER_FLAGS "-sDEMANGLE_SUPPORT=1 --bind -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sDISABLE_EXCEPTION_CATCHING=0")
        set_target_properties(${targetName} PROPERTIES LINK_FLAGS  ${WASM_LINKER_FLAGS})
    endif()

    if (MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${targetName})
        set_property(TARGET ${targetName} PROPERTY VS_GLOBAL_CodeAnalysisRuleSet ${CONFIG_VS_RULESET})

        if (bundle_SRC)
            set_target_properties(${targetName} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${BUNDLE_DIR})
        endif()

    endif()

endmacro()

