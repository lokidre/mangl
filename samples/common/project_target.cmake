
# GUI Type
if (WIN32)
    if ((NOT DEFINED CONFIG_TARGET_CONSOLE) OR ($<NOT:${CONFIG_TARGET_CONSOLE}>))
        set (GUI_TYPE WIN32)
    endif()
endif()


add_executable(${CMAKE_PROJECT_NAME} ${GUI_TYPE} ${main_SRC} ${bundle_SRC})


if (UNIX)
    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
endif()


if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})

    set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_GLOBAL_CodeAnalysisRuleSet ${CONFIG_VS_RULESET})
endif()


# Bundle
if (MSVC)
    if (bundle_SRC)
        set(BUNDLE_DIR "${CMAKE_BINARY_DIR}/bundle")
        file(MAKE_DIRECTORY "${BUNDLE_DIR}")

        foreach (file ${bundle_SRC})
            get_filename_component(targetFile ${file} NAME)
            file(CREATE_LINK "${file}" "${BUNDLE_DIR}/${targetFile}")
        endforeach()

        set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${BUNDLE_DIR})
    endif()
endif()
