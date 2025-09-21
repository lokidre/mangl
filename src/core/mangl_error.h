/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/assert.hpp>


#include <cstdio>
#include <cstdarg>
#include <stdexcept>


//
// seems like everything is obsolete below
//

namespace mangl {

// Lightweight error code/text wrapper
// Doesn't store the actual message. 
struct ErrorInfo {
    int code;
    String text;

    ErrorInfo(): code{0}{}
    ErrorInfo(StrView s): code{1}, text{s}{}
    ErrorInfo(int c, StrView s): code{c}, text{s}{}
    
    template <class ...Args>
    ErrorInfo(StrView fmt, Args&&... args): code{0} {
        text = apl::sformat(fmt, std::forward<Args>(args)...);
    }

    [[nodiscard]] constexpr auto value() const noexcept { return code;  }
    [[nodiscard]] constexpr auto message() const noexcept { return text; }
};

using ErrorInfoA = const ErrorInfo&;


template <class ...Args>
void mthrowErrorBody(const char *const file, int line, Args&&... args) {
    apl::throwError("%s:%d: %s", file, line, apl::sformat(std::forward<Args>(args)...));
}

template <class Condition, class ...Args>
void massertBody(const Condition &cond, const char *const file, int line, Args&&... args) {
    if (!cond)
        mthrowErrorBody(file, line, move(args)...);
}

//template <class B>
//inline 
//void massertBody(const B &b, const char *const file, int line, const char *const str) {
//	if(!b)
//		mthrowErrorBody(file, line, str) ;
//}


template <class ...Args>
[[noreturn]]
void mthrowError(Args&&... args) {
    apl::throwError(std::move(args)...);
}

template <class Condition, class ...Args>
void massert(const Condition &cond, Args&&... args) {
    if (apl::isFalse(cond))
        mthrowError(std::move(args)...);
}


#if _DEBUG

#if _MSVC_TRADITIONAL
#define MDebugThrowError(...)       mthrowErrorBody(__FUNCTION__, __LINE__, __VA_ARGS__)
#define MDebugAssert(_cond, ...)    massertBody(_cond, __FUNCTION__, __LINE__, __VA_ARGS__)
#else
#define MDebugThrowError(...)       mthrowErrorBody(__FUNCTION__, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#define MDebugAssert(_cond, ...)    massertBody(_cond, __FUNCTION__, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#endif

#else // Release

#define MDebugThrowError        ADebugNoop
#define MDebugAssert            ADebugNoop

#endif

}  // namespace mangl

