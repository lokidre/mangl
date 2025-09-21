/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_debug.h"

#include "mangl_color.h"

#include "mangl_framework.h"

#if _DEBUG
#if APEAL_OS_UNIX
#include <pthread.h>
#include <unistd.h>
#endif
#endif


namespace mangl {

namespace M {

static std::size_t debugColorIndex_{};
static constexpr std::array debugColors_ = {
    M::Red, M::Blue, M::Yellow, M::Magenta, M::LightGreen,
};


uint32_t getNextDebugColor() {
    if (debugColorIndex_ >= debugColors_.size())
        debugColorIndex_ = 0;

    return debugColors_[debugColorIndex_++];
}


#ifdef _DEBUG

bool debugAssertMainThread() {
    auto currentThreadId = getCurrentThreadId();
    
    if (currentThreadId != Framework::mainThreadId) {
        MDebugAssert(currentThreadId == Framework::mainThreadId);
        mthrowError("Framework called not from the main thread!");
        //return false;
    }
    
    return true;
}

#endif


}  // namespace M

}  // namespace mangl

