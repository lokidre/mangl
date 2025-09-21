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
#include "../mangl_styles.h"

namespace mangl {

struct SliderParams {
    StrView stateTag;
    WidgetId id;

    Rect rect;

    Texture textures;

    Flags flags;

    bool disabled;
    bool hidden;
    Real position;
    Real granularity;

    SlotReal onPositionBegin;
    SlotReal onPosition;
    SlotReal onPositionEnd;

    SlotIdReal onPositionBeginId;
    SlotIdReal onPositionId;
    SlotIdReal onPositionEndId;


    // Deprecated
    std::function<void(Cord)> positionSignal;
    std::function<void(Cord)> positionBeginSignal, positionEndSignal;
    std::function<void(CommonId, Cord)> positionSignalId;
    std::function<void(CommonId, Cord)> positionBeginSignalId, positionEndSignalId;
};

using SliderParamsA = const SliderParams&;

}  // namespace mangl

