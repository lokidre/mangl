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
#include "mangl_fade_effect_params.h"

namespace mangl {

class FadeEffect: public GuiEffectTemplate<FadeEffect> {
    friend class Scene;

    Real targetAlpha_{};
    Real beginAlpha_{-1};

    FadeEffectP onNew(const FadeEffectParams& params);

public:
    auto setTargetAlpha(Real alpha) {
        targetAlpha_ = static_cast<Color::T>(alpha);
        //beginAlphaSet_ = false;
        return this;
    }

    auto setAlphaRange(Real beginAlpha, Real endAlpha) {
        beginAlpha_ = static_cast<Color::T>(beginAlpha);
        //beginAlphaSet_ = true;
        targetAlpha_ = static_cast<Color::T>(endAlpha);
        return this;
    }

    [[nodiscard]] auto getTargetAlpha() const noexcept { return targetAlpha_; }
    [[nodiscard]] auto getBeginAlpha() const noexcept { return beginAlpha_; }

protected:
    void onStart() override;
    void onStop() override;
    void onTimer(Timestamp lambda) override;
};



}  // namespace mangl

