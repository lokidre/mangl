/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_chain_effect.h"

namespace mangl {


ChainEffect* ChainEffect::push(GuiEffect* effect, Timestamp duration, Timestamp delay) {
    EffectInfo ei;
    ei.effect = effect;
    ei.duration = duration;
    ei.delay = delay;

    // Avoid duplicates
    auto found = false;
    for (auto& e: chainedEffects_) {
        if (e.effect == effect) {
            e = ei;
            found = true;
            break;
        }
    }

    if (!found) {
        chainedEffects_.push_back(ei);
    }

    return this;
}

void ChainEffect::onStart() {
    if (chainedEffects_.empty())
        return;

    // adjust the timing and durations
    // Timestamp startTime = 0 ;
    Timestamp totalDuration = 0;
    for (auto& e: chainedEffects_) {
        totalDuration += e.delay + e.duration;
    }

    if (totalDuration == 0)
        return;

    // Reattach the prims. Just in case
    for (auto& e: chainedEffects_) {
        e.effect->detach();

        if (attachedPrim_)
            attach(attachedPrim_);
        if (attachedWidget_)
            attach(attachedWidget_);
    }


    // Adjust the duration
    auto k = duration_ / totalDuration;

    for (auto& e: chainedEffects_) {
        e.effect->setDuration(e.duration * k);
        e.effect->start(e.delay * k);
    }
}

} // namespace mangl
