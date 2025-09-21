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


class ChainEffect: public GuiEffectTemplate<ChainEffect> {
    using Self = ChainEffect;

public:
    ChainEffect* push(GuiEffect* effect, Timestamp duration, Timestamp delay = 0);
    ChainEffect* clear();


protected:
    void onStart() override;


private:
    struct EffectInfo {
        GuiEffect* effect{};
        Timestamp duration{};
        Timestamp delay{};
        bool removed{};
    };
    std::vector<EffectInfo> chainedEffects_;

    std::vector<EffectInfo> queuedEffects_;
};


} // namespace mangl
