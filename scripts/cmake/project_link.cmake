#

# TODO: remove opengl support from console configurations

if (WIN32)
    # Windows necessary libraries
    link_libraries(winmm.lib Imm32.lib Ws2_32.lib)

    # Minidump and debugging
    link_libraries(dbghelp.lib psapi.lib userenv.lib)

    # Windows OpenGl support
    if (NOT CONFIG_PROJECT_CONSOLE)
        link_libraries(glu32.lib OpenGL32.lib)
    endif()
endif() # Windows

if (LINUX)
    link_libraries(m c dl)
 
    if (NOT CONFIG_PROJECT_CONSOLE)
        link_libraries(GL GLU)
        link_libraries(openal alut)
        link_libraries(EGL)
        #link_libraries(GLESv2)
    endif()
endif()

if (ANDROID)
    set(CMAKE_ANDROID_EXCEPTIONS ON)

    link_libraries(m c dl)
    link_libraries(android mediandk log)

    link_libraries(GLESv3 EGL)
    
    link_libraries(OpenSLES)

    find_package(game-activity REQUIRED CONFIG)
    link_libraries(game-activity::game-activity_static)

#    find_package(oboe REQUIRED CONFIG)
#    link_libraries(oboe::oboe)

endif()


