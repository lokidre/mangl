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
#include "../mangl_texture.h"

namespace mangl {

struct SegmentedSegmentParams {
    String label;
    Texture texture;
};

struct SegmentedParams {

    StrView stateTag;
    Rect rect;

    std::initializer_list<String> labels;
    std::initializer_list<SegmentedSegmentParams> segments;

    int defaultIndex;

    SlotInt onSelect;
    SlotIdInt onSelectId;

    Flags flags;
};

using SegmentedParamsA = const SegmentedParams&;

} // namespace mangl
