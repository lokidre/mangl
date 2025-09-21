/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_effect.h"

namespace mangl {

class BlinkEffect: public GuiEffectTemplate<BlinkEffect> {

    using Self = BlinkEffect;

    Timestamp blinkTimestamp_{};
    bool blinkVisible_{};

    Timestamp blinkRate_{1}, blinkPeriod_{1};

public:
    auto setRate(Timestamp rate) noexcept {
        blinkRate_ = rate;
        blinkPeriod_ = 1_mr/rate;
        return this;
    }


protected:
    void onStart() override;
    void onStop() override;
    void onTimer(Timestamp lambda) override;
};

using BlinkEffectP = BlinkEffect*;

} // namespace mangl

