/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_widget_border.h"

#include "../mangl_scene.h"

namespace mangl {

auto WidgetBorderStrikesRuntime::assign(const ThemeBorder& strikes) -> SelfRet {
    runtimeStrikes.clear();

    radius = strikes.radius;

    for (auto& strike: strikes.strikes) {
        auto& rt = runtimeStrikes.emplace_back();

        rt.color = strike.color;
        rt.width = strike.width;
        rt.offset = strike.offset;
    }

    return *this;
}


auto WidgetBorderStrikesRuntime::create(SceneP scene, Flags flags) -> SelfRet {
    for (auto& strike: runtimeStrikes) {
        auto prim = scene->newRect(strike.color, {}, flags);

        if (radius != 0)
            prim->setRadius(radius);

        prim->setWidth(strike.width);

        strike.prim = prim;
    }

    return *this;
}

auto WidgetBorderStrikesRuntime::move(RectA rectp) -> SelfRet {
    rect = rectp;


    for (auto& strike: runtimeStrikes) {
        auto r = rect;
        r.x += strike.offset;
        r.y += strike.offset;
        r.w -= strike.offset * 2;
        r.h -= strike.offset * 2;

        strike.prim->move(r);
    }

    return *this;
}

auto WidgetBorderStrikesRuntime::setVisible(bool flag) -> SelfRet{
    for (auto& strike: runtimeStrikes) {
        strike.prim->setVisible(flag);
    }

    return *this;
}

auto WidgetBorderStrikesRuntime::setScissor(bool scissorFlag, RectA scissorRect) -> SelfRet {
    for (auto& strike: runtimeStrikes) {
        strike.prim->setScissor(scissorFlag, scissorRect);
    }

    return *this;
}


}  // namespace mangl


