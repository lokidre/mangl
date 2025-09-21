/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../widgets/mangl_widget.h"

namespace mangl {

class DesignGridWidget;
using DesignGridWidgetP = DesignGridWidget*;

struct DesignGridParams {
    // Location
    bool fullscreen;
    Rect rect;

    // Grid layout
    SizeI gridDims;
    Size gridSize;

    // Grid cells setup

    // Grid vertices setup
    PrimType vertexType;
    Size vertexSize;
    CordRange vertexWidthRange;
    CordRange vertexHeightRange;

    Color vertexColor;
    CordRange vertexBrightnessRange;
};

using DesignGridParamsA = const DesignGridParams&;

}  // namespace mangl
