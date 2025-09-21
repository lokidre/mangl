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

struct CrossfadeEffect;
using CrossfadeEffectP = CrossfadeEffect*;

struct CrossfadeEffectParams {
    PrimP prim{};
    WidgetP widget{};

    Color color1{};
    Color color2{};

    Timestamp rate{};
    Timestamp duration{};
    bool permanent{};

    Timestamp period{};

    bool repeat{};
    bool bounce{true};
    bool reverse{};
    bool disappear{};

    Easing easing{Easing::None};
};

} // namespace mangl
