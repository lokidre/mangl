#

#message( "prj: ${CMAKE_PROJECT_NAME}" )

# Build configurations
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type specified. Defaulting to Debug.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

if(CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Available build configurations: ${CMAKE_CONFIGURATION_TYPES}")
else()
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()


if (UNIX AND NOT APPLE AND NOT ANDROID AND NOT EMSCRIPTEN)
    set(LINUX YES)
endif()


# Set C++ Language Standard
enable_language(CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_C_STANDARD 17)


if ("${CMAKE_PROJECT_NAME}" EQUAL "Project")
    message(FATAL_ERROR, "Set project before including this file")
endif()

include_directories(${CONFIG_INCLUDE_DIRECTORIES})
link_directories(${CONFIG_LINK_DIRECTORIES})

#if (NOT ANDROID)
#    include_directories($ENV{APEAL_INCLUDE_DIRECTORIES})
#    link_directories($ENV{APEAL_LINK_DIRECTORIES})
#endif()



option(FORCE_UNIX "Force Unix compilation")

if (LINUX)
    # apt install pkg-config
    find_package(PkgConfig REQUIRED)

    if (NOT CONFIG_MANGL_SDL AND NOT CONFIG_MANGL_GTK)
        set(CONFIG_MANGL_SDL YES)
        # message("sdl enabled")
    endif()

endif()

if (CONFIG_PROJECT_FORCE_UNIX OR FORCE_UNIX)
    add_definitions(-DTARGET_OS_POSIX=1)
endif()


#
# Project configurations
#
if (CONFIG_PROJECT_CONSOLE OR CONFIG_TARGET_CONSOLE)
    set(CONFIG_TARGET_CONSOLE on)

    add_compile_definitions(MANGL_CONSOLE=1)
    add_compile_definitions(MANGL_NO_MAIN=1)
endif()

if (CONFIG_MANGL_NO_MAIN)
    add_compile_definitions(MANGL_NO_MAIN=1)
endif()


if (CONFIG_PROJECT_MULTIPLAYER)
    add_compile_definitions(CONFIG_PROJECT_MULTIPLAYER=1)
    add_compile_definitions(MANGL_MULTIPLAYER=1)

    if (NOT APPLE)
        set(CONFIG_MANGL_IXWEBSOCKET ON)
        set(CONFIG_MANGL_UWEBSOCKETS ON)
        # if (NOT CONFIG_MANGL_IXWEBSOCKET)
        #     set(CONFIG_MANGL_UWEBSOCKETS ON)
        # endif()
    endif()

    if (CONFIG_MANGL_IXWEBSOCKET)
        add_compile_definitions(MANGL_IXWEBSOCKET=1)
    endif()

    if (CONFIG_MANGL_UWEBSOCKETS)
        add_compile_definitions(MANGL_UWEBSOCKETS=1)
        #add_compile_definitions(UWS_NO_ZLIB=1)  ## TODO: fix to use builtin zlib
    endif()

    #add_compile_definitions(MANGL_BOOST_BEAST=1)

endif()

if (CONFIG_PROJECT_BLUETOOTH)
    add_compile_definitions(MANGL_BLUETOOTH=1)

    if (LINUX)
        add_compile_definitions(MANGL_BLUETOOTH_DBUS=1)

        # apt install libsystemd-dev
        pkg_check_modules(SYSTEMD REQUIRED libsystemd)
        link_libraries(${SYSTEMD_LINK_LIBRARIES})

        find_package(sdbus-c++ REQUIRED)
        link_libraries(SDBusCpp::sdbus-c++)

    endif()

    if (ANDROID)
        add_compile_definitions(MANGL_BLUETOOTH_JNI=1)
    endif()
endif()

if (CONFIG_PROJECT_NO_CONFIG_H)
    add_compile_definitions(MANGL_NO_CONFIG_H=1)
endif()

if (CONFIG_PROJECT_UNIX_DAEMON) 
    if (UNIX)
        # apt install libsystemd-dev
        pkg_check_modules(SYSTEMD REQUIRED libsystemd)
        link_libraries(${SYSTEMD_LINK_LIBRARIES})
    endif()
endif()



# # Adding coloring output to GNU and Clang
# add_compile_options($<$<CXX_COMPILER_ID:GNU>:>)
# add_compile_options($<$<CXX_COMPILER_ID:Clang>:-fcolor-diagnostics>)

add_compile_definitions($<$<CONFIG:Debug>:_DEBUG=1>)  # debug flag
add_compile_definitions(__STDC_WANT_LIB_EXT1__=1) # request bound checked functions


#message ( "msvc: '${MSVC}' comp id: '${CMAKE_CXX_COMPILER_ID}'" )


if (NOT EMSCRIPTEN)
    find_package(Threads REQUIRED)
    link_libraries(Threads::Threads)
endif()

if (EMSCRIPTEN)
    add_compile_options(-sDISABLE_EXCEPTION_CATCHING=0 -sUSE_ZLIB=1)
endif()


if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fexceptions)
    add_compile_options(-fcolor-diagnostics)

    #add_compile_options(-Wmissing-field-initializers)
    #add_compile_options(-Wmissing-designated-field-initializers)

endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_options(-fexceptions)
    add_compile_options(-fdiagnostics-color=always)
endif()


if (LINUX)
    # Valgrind bug fix
    add_compile_options(-gdwarf-4)
endif()


if (ANDROID)
    include_directories(${ANDROID_NDK}/sources/android/native_app_glue)
    include_directories(${CONFIG_MANGL_DIR}/src/android/3rdparty/games-frame-pacing/include)
    include_directories(${CONFIG_MANGL_DIR}/src/android/3rdparty/games-frame-pacing/common)
    include_directories(${CONFIG_MANGL_DIR}/src/android/3rdparty/gamesdk/common)
    include_directories(${CONFIG_MANGL_DIR}/src/android/3rdparty/parselib)
    include_directories(${CONFIG_MANGL_DIR}/src/android/3rdparty/iolib)

    add_definitions(-DANDROIDGAMESDK_NO_BINARY_DEX_LINKAGE)

    add_compile_options(-fexperimental-library)

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u Java_com_google_androidgamesdk_GameActivity_initializeNativeCode")

#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O0 -gline-tables-only")

    # Android optimization level
    #set(CPP_OPTIMIZATION 1)
    # set(CPP_OPTIMIZATION 2)
    set(CPP_OPTIMIZATION 3)

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O${CPP_OPTIMIZATION}")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O${CPP_OPTIMIZATION}")

endif()



if (WIN32)  # Windows is always Unicode
    add_compile_definitions(_UNICODE UNICODE)
    add_compile_options(-U_MBCS)
endif()


if (MSVC)
    add_compile_options(/EHsc)  # catch only C++ exceptions, assume extern C never throw exceptions
    add_compile_options(/MP)  # Enable multiprocessor compilation

    # cmake_host_system_information(RESULT processorCount QUERY NUMBER_OF_PHYSICAL_CORES)
    # if ("${processorCount}" > 2)
    #     set(mpCount ${processorCount})
    #     math(EXPR mpCount "${mpCount} - 2")
    #     add_compile_options("/MP${mpCount}")  # Enable multiprocessor compilation
    # else()
    #     add_compile_options(/MP)  # Enable multiprocessor compilation
    # endif()


    #
    # Debug configuration
    #
    add_compile_options($<$<CONFIG:Debug>:/MDd>)  # Code generation Debug DLL
    add_compile_options($<$<CONFIG:Debug>:/fp:except>)  # Enable floating point exceptions
    #add_compile_options ($<$<CONFIG:Debug>:/FR>)  # Enable browser information

    #
    # Language tuning
    #
    add_compile_options(/Zc:rvalueCast)     # Enforce type conversion rules
    #add_compile_options(/Zc:preprocessor)   # C++20 __VA_OPT__ preprocessor support
    add_compile_options(/permissive-)       # Make standard conformant
    add_compile_options("/bigobj")       # Large number of entries support support
    #add_compile_options("/showIncludes")  # show include tree



    # Eliminating warning: defaultlib 'library' conflicts with use of other libs; use /NODEFAULTLIB:library
    # Multithreaded (libcmt.lib) ignore:  msvcrt.lib; libcmtd.lib; msvcrtd.lib
    # Multithreaded using DLL (msvcrt.lib)  libcmt.lib; libcmtd.lib; msvcrtd.lib
    # Debug Multithreaded (libcmtd.lib) libcmt.lib; msvcrt.lib; msvcrtd.lib
    # Debug Multithreaded using DLL (msvcrtd.lib)   libcmt.lib; msvcrt.lib; libcmtd.lib
    #add_link_options($<$<CONFIG:Debug>:/NODEFAULTLIB:msvcrt.lib> $<$<CONFIG:Debug>:/NODEFAULTLIB:msvcrt.lib> $<$<CONFIG:Debug>:/NODEFAULTLIB:libcmtd.lib>)
    add_link_options($<$<CONFIG:Debug>:/NODEFAULTLIB:libcmt.lib> $<$<CONFIG:Debug>:/NODEFAULTLIB:msvcrt.lib> $<$<CONFIG:Debug>:/NODEFAULTLIB:libcmtd.lib>)

    #
    # Warnings. Enable all warnings and just disable specific ones
    #
    add_compile_options(/W4)
    add_compile_options(/wd4100) # 'unreferenced formal parameter'
    add_compile_options(/wd4206) # nonstandard extension used: translation unit is empty
    add_compile_options(/wd4251) # class '' needs to have dll-interface to be used by clients of class
    add_compile_options(/wd4505) # unreferenced local function has been removed
    add_compile_options(/wd4189) # local variable is initialized but not referenced
    add_compile_options(/wd4324) # structure was padded due to alignment specifier

    #
    # Optimized configurations
    #
    #add_compile_options($<$<NOT:$<CONFIG:Debug>>:/MT>)  # Static linking for Release
    add_compile_options($<$<NOT:$<CONFIG:Debug>>:/MD>)  # Dynamic linking for Release
endif()

if (APPLE)
    add_compile_options(-fmodules -fcxx-modules)
endif()



if (CONFIG_MANGL_SDL)
    add_compile_definitions(MANGL_SDL=1)
    add_compile_definitions(SDL_MAIN_HANDLED)
      
    # Try to use SDL3 and fallback to SDL2
    find_package(SDL3 QUIET)

    if (${SDL3_FOUND})
        add_compile_definitions(MANGL_SDL3=1)
        include_directories(${SDL3_INCLUDE_DIRS})
        link_directories(${SDL3_LIBRARY_DIRS})
        link_libraries(${SDL3_LIBRARIES})

    else()
    
        find_package(SDL2 REQUIRED)
        add_compile_definitions(MANGL_SDL2=1)
        include_directories(${SDL2_INCLUDE_DIRS})
        link_directories(${SDL2_LIBRARY_DIRS})
        link_libraries(${SDL2_LIBRARIES})
    endif()

endif()


#
# Convenience macros
#
macro (addMainSrc sources)
    set(src ${sources} ${ARGN} )

    list(APPEND main_SRC ${src})
endmacro()
