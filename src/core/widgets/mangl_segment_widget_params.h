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

struct SegmentWidgetParams {
    SegmentWidgetP segment;
    String text;
    Texture icon;

    WidgetP widget;
    WidgetP pressedWidget;
    WidgetP selectedWidget;

    Texture segmentTex;
    Texture pressedTex;
    Texture selectedTex;

    Texture segmentFirstTex;
    Texture pressedFirstTex;
    Texture selectedFirstTex;

    Texture segmentLastTex;
    Texture pressedLastTex;
    Texture selectedLastTex;

};

} // namespace mangl
