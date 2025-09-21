/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_types.h"

namespace mangl {

constexpr static int MaxTouches = 8;

struct TouchInfo {
    Point p;  // Touch position
    Timestamp timestamp;  // When the touch happened
    int index;  // Touch index
    int count;  // How many touches are down
};

using TouchInfoA = const TouchInfo&;
using TouchA = TouchInfoA;


struct TouchesInfo {
    constexpr static int MaxTouches = 8;
    int count;
    std::array<TouchInfo, MaxTouches> touches;
};

using TouchesInfoA = const TouchesInfo&;
using TouchesA = const TouchesInfo&;

}  // namespace mangl
