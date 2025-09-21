#

# TODO: remove opengl support from console configurations
include(${CMAKE_CURRENT_LIST_DIR}/project_link.cmake)

# GUI Type
if (WIN32)
    if ((NOT DEFINED CONFIG_TARGET_CONSOLE) OR ($<NOT:${CONFIG_TARGET_CONSOLE}>))
        set (GUI_TYPE WIN32)
    endif()
endif()

if (ANDROID)
    add_library(${CMAKE_PROJECT_NAME} SHARED ${main_SRC} ${bundle_SRC})
else()
    add_executable(${CMAKE_PROJECT_NAME} ${GUI_TYPE} ${main_SRC} ${bundle_SRC})
endif()

if (APPLE)
    # Enable ARC for all Apple systems
    set_property (TARGET ${CMAKE_PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS "-fobjc-arc")

    # Add MacOS frameworks
    if (NOT IOS)
        find_library(CORE_FOUNDATION_FW CoreFoundation)
        find_library(FOUNDATION_FW Foundation)
        find_library(APP_KIT_FW AppKit)

        mark_as_advanced(${CORE_FOUNDATION_FW} ${FOUNDATION_FW} ${APP_KIT_FW})
        set(MACOS_LIBS ${CORE_FOUNDATION_FW} ${FOUNDATION_FW} ${APP_KIT_FW})
        target_link_libraries(${CMAKE_PROJECT_NAME} ${MACOS_LIBS})
    endif()
endif()


if (UNIX)
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE TRUE)    
endif()


if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_GLOBAL_CodeAnalysisRuleSet ${CONFIG_VS_RULESET})
endif()

if (EMSCRIPTEN)
    set(WASM_LINKER_FLAGS "-sDEMANGLE_SUPPORT=1 --bind -sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sDISABLE_EXCEPTION_CATCHING=0")
#    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --preload-file ${CMAKE_SOURCE_DIR}/assets --bind")
    #set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --bind")
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS ${WASM_LINKER_FLAGS})
endif()



# Bundle
if (bundle_SRC)
    if (ANDROID)
        # foreach (file ${bundle_SRC})
        #     get_filename_component(targetFile ${file} NAME)
        #     add_custom_command(
        #         TARGET ${CMAKE_PROJECT_NAME}
        #         POST_BUILD
        #         COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        #             "${file}"
        #             "${CMAKE_ANDROID_ASSETS_DIRECTORIES}/${targetFile}"
        #         COMMENT "Copy ${targetFile}"
        #     )
        # endforeach()
    else()
        set(BUNDLE_DIR "${CMAKE_BINARY_DIR}/bundle")
        file(MAKE_DIRECTORY "${BUNDLE_DIR}")

        foreach (bundleFile ${bundle_SRC})
            get_filename_component(fileName ${bundleFile} NAME)
            if (WIN32)
		        # string(REPLACE "/" "\\" TGT "${BUNDLE_DIR}/${fileName}")
		        # string(REPLACE "/" "\\" SRC "${bundleFile}")

                # if (NOT EXISTS "${BUNDLE_DIR}/${targetFile}")
                #     execute_process(COMMAND cmd.exe /c mklink /h "${TGT}" "${SRC}")
                # endif()
                file(CREATE_LINK "${bundleFile}" "${BUNDLE_DIR}/${fileName}")
            else()
                file(CREATE_LINK "${bundleFile}" "${BUNDLE_DIR}/${fileName}" SYMBOLIC)
            endif()
        endforeach()
    endif()
endif()


if (MSVC)
    if (bundle_SRC)
        # set(BUNDLE_DIR "${CMAKE_BINARY_DIR}/bundle")
        # file(MAKE_DIRECTORY "${BUNDLE_DIR}")

        # foreach (file ${bundle_SRC})
        #     get_filename_component(targetFile ${file} NAME)
        #     file(CREATE_LINK "${file}" "${BUNDLE_DIR}/${targetFile}")
        # endforeach()

        set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${BUNDLE_DIR})
    endif()
endif()
