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
#include "../mangl_styles.h"

namespace mangl {

struct BorderStrikeRuntime {
    Color color{};
    Cord width{};
    Cord offset{};
    PrimP prim{};
};

struct WidgetBorderStrikesRuntime {
    using SelfRet = WidgetBorderStrikesRuntime&;

    std::vector<BorderStrikeRuntime> runtimeStrikes;
    Cord radius{};
    Rect rect{};

    SelfRet assign(const ThemeBorder& strikes);
    SelfRet create(SceneP scene, Flags flags);
    SelfRet move(RectA rect);
    SelfRet setVisible(bool flag);
    SelfRet setScissor(bool scissorFlag, RectA scissorRect);
};


}  // namespace mangl
