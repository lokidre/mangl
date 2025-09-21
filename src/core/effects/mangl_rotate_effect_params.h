/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"
#include "mangl_easing.h"


namespace mangl {

class RotateEffect;
using RotateEffectP = RotateEffect*;

struct RotateEffectParams {
    PrimP prim{};
    WidgetP widget{};
    Cord angle{};

    Point offset{};

    //
    // Common params for all effects
    //
    Timestamp rate{};

    Timestamp duration{};
    bool permanent{true};

    Timestamp period{};

    bool repeat{};
    bool bounce{};
    bool reverse{};
    bool disappear{};


    Easing easing{Easing::None};
};

using RotateEffectParamsA = const RotateEffectParams&;

}  // namespace mangl
