function(addTarget targetName targetDir targetFiles)
    #cmake_parse_arguments(PARSE_ARGV 3 COMPLEX_PREFIX)

    # GUI Type
    if (WIN32)
        if ((NOT DEFINED CONFIG_TARGET_CONSOLE) OR ($<NOT:${CONFIG_TARGET_CONSOLE}>))
            set (GUI_TYPE WIN32)
        endif()
    endif()

    add_executable(${targetName} ${GUI_TYPE} ${main_SRC} ${bundle_SRC})

    if (UNIX)
        set_property(TARGET ${targetName} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
    endif()


endfunction()
