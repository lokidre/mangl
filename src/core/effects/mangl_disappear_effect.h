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
#include "mangl_disappear_effect_params.h"

namespace mangl {

struct DisappearEffect: public GuiEffectTemplate<DisappearEffect> {
    friend class Scene;

    DisappearEffectP onNew(const DisappearEffectParams& params);

protected:
    virtual void onStop() override;
};

using DisappearEffectP = DisappearEffect*;

}  // namespace mangl
