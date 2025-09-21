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

namespace mangl{

struct ScaleEffectParams {

    PrimP prim{};
    WidgetP widget{};
    Cord scale{};

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


}//namespace mangl