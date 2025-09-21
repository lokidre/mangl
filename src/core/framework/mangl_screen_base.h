/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"
#include "../mangl_keyboard.h"


namespace mangl {

class ScreenBase {
    friend class Framework;
    friend struct ScreenTransitionContext;

    bool frameworkKeyBegan_{false};
    bool frameworkBackProcessed_{true};

protected:
    virtual ~ScreenBase() = default;


    virtual void onStartPrefix() {}
    virtual void onStart() {}
    virtual void onStartPostfix() {}

    virtual void onReleasePrefix() {}
    virtual void onRelease() {}
    virtual void onReleasePostfix() {}

    virtual void onPause() {}
    virtual void onResume() {}
    virtual void onResignActive() { onPause(); }
    virtual void onResumeActive() { onResume(); }
    virtual void onTerminate() {}

    virtual void onTimerPrefix(Timestamp) {}
    virtual void onTimer(Timestamp) {}
    virtual void onTimerPostfix(Timestamp) {}

    virtual void onTransitionBegin(bool in) {}
    virtual void onTransition(bool in, Timestamp lambda) {}
    virtual void onTransitionEnd(bool in) {}

    //
    // Touches
    //
    virtual void onTouchDown(TouchA) {}
    virtual void onTouchMove(TouchA) {}
    virtual void onTouchUp(TouchA) {}
    virtual void onTouchCancel(TouchA) {}

    virtual void onTouchUpPostfix(TouchA) {}

    //
    // Keyboard
    //
    virtual void onKeyDown(KeyCode) {}
    virtual void onKeyUp(KeyCode) {}
    virtual void onBack() { frameworkBackProcessed_ = false; }

    //
    // Mouse
    //
    virtual void onMouseWheel(Cord delta, Flags keys, Point pos) {}

    // General calls
    virtual String onCall(StrView name, StrView data){ return {}; }

};

using ScreenBaseP = ScreenBase*;

}  // namespace mangl
