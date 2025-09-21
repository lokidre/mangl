/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#define APEAL_NO_DEBUG_CONSOLE 1

#include <apeal/debug.hpp>
#include "main/mangl_thread.h"


#ifdef __OBJC__

#ifdef _DEBUG
#define DLog(msg, ...) NSLog((msg), ##__VA_ARGS__)
#else
#define DLog(msg, ...) (void(0))
#endif

#endif  // __OBJC__ - Objective C

namespace mangl {

constexpr auto MDebugPrefix = "Mangl: ";

#define MDebugNoop          ADebugNoop

#define MDebugTrap          ADebugTrap
#define MDebugTrapAssert    ADebugTrapAssert

//#define MDebugPrint         ADebugPrint
#define MDebugFunc          ADebugPrintFunc


#if _DEBUG
#if _MSVC_TRADITIONAL
#define MDebugPrint(...)    ADebugPrintPrefix((MDebugPrefix), __VA_ARGS__)
//#define MDebugFunc(...)     ADebugPrintFuncPrefix(MDebugPrefix, __VA_ARGS__)
#else
#define MDebugPrint(...)    ADebugPrintPrefix((MDebugPrefix) __VA_OPT__(,) __VA_ARGS__)
//#define MDebugFunc(...)     ADebugPrintFuncPrefix(MDebugPrefix __VA_OPT__(,) __VA_ARGS__)
#endif
#else // Release
#define MDebugPrint         ADebugNoop
//#define MDebugFunc          ADebugNoop
#endif


namespace M {
uint32_t getNextDebugColor();

#if _DEBUG
inline ThreadId debugGetCurrentThreadId() { return getCurrentThreadId(); }
bool debugAssertMainThread();
#else
constexpr inline ThreadId debugGetCurrentThreadId(){ return 0; }
constexpr inline bool debugAssertMainThread() {  return true; }
#endif

};

//
// Debugging screen layouts
//
enum class DebugScreenLayout {
    Unknown,
    iPhone,   // 320 x 480
    iPhone5,  // 320 x 568
};


} // namespace mangl
