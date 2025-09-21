/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_element.h"

namespace mangl::mmp {

struct MpPacketCharacter {
    unsigned key;  // Element unique key
    unsigned player;  // Which player this element belongs to

    PointI position;  // Element location in index
    Point point;  // Element position
    SizeI dims;  // Element dimensions
    Size size;  // Element size
    RectI box; 
    Rect rect;

    unsigned color;

    MpPacketElementsContainer elements;

    void* rtContext;  // Runtime not be transmitted
};

using MpPacketElementsContainer = std::vector<MpPacketElement>;


}  // namespace mangl::mmp

