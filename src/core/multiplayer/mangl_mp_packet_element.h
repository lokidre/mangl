/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_types.h"

namespace mangl::mmp {

struct MpPacketElement {
    unsigned key;  // Element unique key
    unsigned player;  // Which player this element belongs to

    unsigned type;  // Element type arbitrary
    unsigned color;  // Element color
    unsigned state;  // Element state

    PointI position;  // Element location in index
    PointI relativePosition;  // Element relative position to the other item
    Point point;  // Element position
    SizeI dims;  // Element dimensions
    Size size;  // Element size
    RectI box; 
    Rect rect;

    void* rtContext;  // Runtime not be transmitted
};

using MpPacketElementsContainer = std::vector<MpPacketElement>;

struct MpPacketElementGroup {
    unsigned type;
    MpPacketElementsContainer elements;
};

using MpPacketElementGroupContainer = std::vector<MpPacketElementGroup>;

}  // namespace mangl::mmp

