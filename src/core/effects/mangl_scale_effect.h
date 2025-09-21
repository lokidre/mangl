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
#include "mangl_scale_effect_params.h"

namespace mangl {


struct ScaleEffect: public GuiEffectTemplate<ScaleEffect> {
    friend class Scene;

public:
    auto setScale(Cord scale) noexcept {
        scale_ = scale;
        return this;
    }

private:
    Cord scale_{1};
    Cord beginScale_{}, endScale_{};

    struct SavedInfo {
        Rect r{};
        PrimP prim{};
    } savedInfo_;

    ScaleEffectP onNew(const ScaleEffectParams& params);

protected:
    void onStart() override;
    void onStop() override;
    void onTimer(Timestamp lambda) override;
};

using ScaleEffectP = ScaleEffect*;

}  // namespace mangl
