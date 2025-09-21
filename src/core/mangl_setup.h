/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/def.hpp>
#include <apeal/cprep.hpp>

#ifndef MANGL_NO_CONFIG_H
#ifdef MANGL_CONFIG_H
#include APEAL_PP_QUOTE(MANGL_CONFIG_H)
#else
#include <mangl_config.h>
#endif
#endif


#if APEAL_OS_ANDROID

#define MANGL_AUDIO_OPENSL  1
#define MANGL_MUSIC_JNI     1
//#define MANGL_AUDIO_OBOE 1
//#define MANGL_MUSIC_OBOE 1

#elif APEAL_OS_WINDOWS

#define MANGL_AUDIO_DIRECTSOUND 1
#define MANGL_MUSIC_MCI         1

#elif APEAL_OS_MAC

//#if APEAL_OS_IPHONE
#define MANGL_AUDIO_OPENAL      1
//#else  // for Desktop
//#define MANGL_AUDIO_AVAUDIOENGINE 1
//#endif

#define MANGL_MUSIC_AUDIOQUEUE  1
//#define MANGL_MUSIC_AUDIOUNIT  1

//#elif defined(__EMSCRIPTEN__)

// No audio for emscripten
// there is no alut in emscripten

#elif APEAL_OS_UNIX

#define MANGL_AUDIO_OPENAL      1

#endif


#if MANGL_ADS
    #if !defined(MANGL_ADS_BANNER) && !defined(MANGL_ADS_INTERSTITIAL)
        #error "MANGL_ADS defined, but neither MANGL_ADS_BANNER nor MANGL_ADS_INTERSTITIAL are"
    #endif
#endif



//
// Renderer
//
#if APEAL_OS_IPHONE

#if !defined(MANGL_OPENGL) && !defined(MANGL_OPENGL_ES3)
    #define MANGL_METAL 1
#endif


#ifndef MANGL_METAL
    #define MANGL_OPENGL        1
    #define MANGL_OPENGL_ES     1
    #define MANGL_OPENGL_ES3    1
#endif

#elif APEAL_OS_MAC

#define MANGL_METAL         1
//#define MANGL_OPENGL        1
//#define MANGL_OPENGL_ES     1
//#define MANGL_OPENGL_ES3    1

#elif APEAL_OS_ANDROID

#define MANGL_OPENGL        1
#define MANGL_OPENGL_ES     1
#define MANGL_OPENGL_ES3    1

#elif APEAL_OS_WINDOWS

#ifdef MANGL_METAL
    #undef MANGL_METAL
#endif

#if MANGL_SDL
//#define SDL_MAIN_HANDLED
#else
#define MANGL_WINMAIN 1
#endif

#ifndef MANGL_CONSOLE
#define MANGL_OPENGL        1
#endif

#elif defined(__EMSCRIPTEN__)

#define MANGL_OPENGL        1
#define MANGL_OPENGL_ES     1
#define MANGL_OPENGL_ES3    1

#elif APEAL_OS_LINUX

#define MANGL_OPENGL        1

// Defaults to SDL under linux
#if !defined(MANGL_CONSOLE) && !defined(MANGL_GTK) && !defined(MANGL_SDL)
    #define MANGL_SDL 1
#endif

#endif

//
// Multiplayer
//
#if MANGL_MULTIPLAYER

#if APEAL_OS_WINDOWS || APEAL_OS_LINUX



#if !MANGL_IXWEBSOCKET && !MANGL_UWEBSOCKETS
    #define MANGL_IXWEBSOCKET 1
    #define MANGL_UWEBSOCKETS 1
#endif

#endif  // OS
#endif  // Multiplayer


#define MANGL_WIN_GAME_LOOP_TIMER_SLEEP 1
//#define MANGL_WIN_GAME_LOOP_TIMER_MM    1
