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

struct WidgetLayoutParams {
    Cord left{M::InvalidCord};
    Cord top{M::InvalidCord};
    Cord right{M::InvalidCord};
    Cord bottom{M::InvalidCord};
    Cord width{M::InvalidCord};
    Cord height{M::InvalidCord};
    Size size{M::InvalidCoord, M::InvalidCoord};
    Cord ratio{};
    Flags align{};
    //Flags flags{};
};

using WidgetLayoutParamsA = const WidgetLayoutParams&;

struct WidgetLayout {

    void setParams(WidgetLayoutParamsA params);
    void calcRect(RectA ir, Rect* out, M::Orientation o = M::Orientation0);

    Rect calcRect(RectA ir, M::Orientation o = M::Orientation0) {
        Rect out{};
        calcRect(ir, &out, o);
        return out;
    }


private:

    WidgetLayoutParams params_;
    bool fullLayout_ {};

    Cord getHorzValue(Cord val, RectA r) noexcept {
        if (std::fabs(val) < 1)
            return r.w * val;
        return val;
    }

    Cord getVertValue(Cord val, RectA r) noexcept {
        if (std::fabs(val) < 1)
            return r.h * val;
        return val;
    }


};

}  // namespace mangl
