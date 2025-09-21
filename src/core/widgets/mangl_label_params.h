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

namespace mangl {

enum class LabelEffectType {
    None,
    Shadow,
    OuterGlow,
};

struct LabelEffectParams {
    LabelEffectType type{LabelEffectType::None};
    Color color;
    Point offset;
    Cord blur;
    Cord scale;
    Cord opacity;
};

using LabelEffectParamsA = const LabelEffectParams&;

struct LabelBackgroundEffectParams {
    Color color;
    Point offset{};
    Cord scale{1};
};

struct LabelOuterGlowEffectParams {
    Color color;
    Cord blur{4};
    Cord opacity{0.75};
};

struct TextStyle;

struct LabelParams {
    const TextStyle* style;
    
    String text;
    int textInt;
    Point pos;
    Rect rect;


    const Font* font;
    Color color;
    Real alpha;
    bool transparent;
    Cord textSize;
    Cord spacing;
    Flags align;
    bool fit;
    LabelEffectParams effect;

    bool hidden;
    Flags flags;
};

using LabelParamsA = const LabelParams&;

}  // namespace mangl
