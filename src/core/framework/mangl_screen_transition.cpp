/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_screen_transition.h"

#include "../mangl_framework.h"
#include "../mangl_scene.h"


namespace mangl {

void ScreenTransitionContext::setParams(ScreenTransitionParamsA params) {
    params_ = params;

    if (params_.transition != Transition::Unknown)
        transition = params.transition;

    duration = params_.duration;
    if (duration <= 0)
        duration = 0.8; // default duration

    if (params_.easing == Easing::Invalid) {
        //params_.easing = Easing::QuartInOut;  // Default easing
        params_.easing = Easing::CubicInOut;  // Default easing
    }

    if (params_.scene)
        scenes_.push_back(params_.scene);
    else
        scenes_.push_back(Framework::getScene());
}

void ScreenTransitionContext::start(ScreenBaseP screen, Timestamp timestamp) {
    screen_ = screen;
    beginTimestamp_ = timestamp;
    wasInteractionEnabled_ = Framework::setInteractionEnabled(false);
    active = true;
}

void ScreenTransitionContext::processTimer(Timestamp timestamp) {
    if (!active) return;
    if (timestamp >= beginTimestamp_ + duration) {
        finish();
        return;
    }

    auto lambda = (timestamp - beginTimestamp_) / duration;

    auto l = lambda;
    if(!direction) l = 1 - l;

    if (screen_) screen_->onTransition(direction, lambda);

    for (auto scene: scenes_) {
        switch (transition) {
            using enum Transition;
        case Fade:
            scene->setAlpha(M::ease(params_.easing, l));
            break;
        default:
        MDebugTrap();
            break;
        }
    }
}


void ScreenTransitionContext::finish() {
    if (!active) return;
    if (screen_) screen_->onTransitionEnd(direction);

    for (auto scene: scenes_) {
        switch (transition) {
            using enum Transition;
        case Fade:
            if (direction)
                scene->clearAlpha();
            else
                scene->setAlpha(true, 0);
            break;
        default:
            MDebugTrap();
            break;
        }
    }

    Framework::setInteractionEnabled(wasInteractionEnabled_);

    active = false;
}


}  // namespace mangl

