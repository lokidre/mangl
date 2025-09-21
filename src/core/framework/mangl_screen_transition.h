/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_transition.h"
#include "../mangl_scene_decl.h"
#include "mangl_screen_base.h"
#include "../effects/mangl_easing.h"

namespace mangl {


struct ScreenTransitionParams {
    Transition transition{Transition::None};
    Timestamp duration{0};
    Easing easing{Easing::Invalid};
    SceneP scene{};
};

using ScreenTransitionParamsA = const ScreenTransitionParams&;

struct ScreenTransitionContext {

    // Setup
    Transition transition{Transition::None};
    Timestamp duration{1};
    bool direction{};  // true - in, false - out

    // Status
    bool active{};


    // Runtime stuff
    std::vector<SceneP> scenes_;
    ScreenBaseP screen_{};

    ScreenTransitionParams params_;
    Timestamp lambda_{0};
    Timestamp beginTimestamp_{};
    bool wasInteractionEnabled_{};

    void setParams(ScreenTransitionParamsA params);
    void start(ScreenBaseP screen, Timestamp timestamp);
    void processTimer(Timestamp timestamp);
    void finish();
};


}  // namespace mangl

