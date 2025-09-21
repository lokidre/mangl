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
#include "mangl_rotate_effect_params.h"

namespace mangl {


class RotateEffect: public GuiEffectTemplate<RotateEffect> {
    friend class Scene;

    Cord currentAngle_;

    constexpr static Cord pi_ = cognit::constants::pi<Real>;

    Cord maxAngle_ = pi_ * 2;

    struct SavedInfo {
        Cord beginRotation{};
        PrimP prim{};
    } savedInfo_;

    RotateEffectP onNew(RotateEffectParamsA params);


public:
    auto setAngle(Cord angle) noexcept {
        maxAngle_ = angle;
        return this;
    }

protected:
    void onStart() override;
    void onStop() override;
    void onTimer(Timestamp lambda) override;
};

} // namespace mangl
