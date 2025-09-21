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
#include "mangl_crossfade_effect_params.h"

namespace mangl {

struct CrossfadeEffect: public GuiEffectTemplate<CrossfadeEffect> {
    friend class Scene;

    using Self = CrossfadeEffect;

    //auto setRate(Timestamp rate) noexcept {
    //    //crossfadePeriod_ = Timestamp(1) / rate;
    //    this->setPeriod(1/rate);
    //    return this;
    //}

    auto setColors(ColorA color1, ColorA color2) noexcept {
        colors_[0] = color1;
        colors_[1] = color2;
        return this;
    }

private:
    Color colors_[2];
    //Timestamp crossfadePeriod_{1};

    struct SavedInfo {
        bool colorFlag{};
        Color color;
        PrimP prim{};
        WidgetP widget{};
    } savedInfo_;


    CrossfadeEffectP onNew(const CrossfadeEffectParams& params);

protected:
    void onStart() override;
    void onStop() override;
    void onTimer(Timestamp lambda) override;
};

using CrossfadeEffectP = CrossfadeEffect*;

}  // namespace mangl
