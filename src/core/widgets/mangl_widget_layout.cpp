/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_widget_layout.h"

namespace mangl {

void WidgetLayout::setParams(WidgetLayoutParamsA params) {
    params_ = params;

    if (!M::isInvalidCord(params_.size.w)) params_.width = params_.size.w;
    if (!M::isInvalidCord(params_.size.h)) params_.height = params_.size.h;

    if (M::isInvalidCord(params_.width) && M::isInvalidCord(params_.height) &&
        M::isInvalidCord(params_.left) && M::isInvalidCord(params_.top) &&
        M::isInvalidCord(params_.right) && M::isInvalidCord(params_.bottom)) {
        fullLayout_ = true;
    }
}


void WidgetLayout::calcRect(RectA ir, Rect* out, M::Orientation o) {
    if (fullLayout_) {
        *out = ir;
        return;
    }



    if (M::isValidCord(params_.width))
        out->w = getHorzValue(params_.width, ir);
    else if (M::isValidCord(params_.left) && M::isValidCord(params_.right))
        out->w = (ir.w - getHorzValue(params_.right, ir)) - getHorzValue(params_.left, ir);
    else
        out->w = 0;


    if (M::isValidCord(params_.height))
        out->h = getHorzValue(params_.height, ir);
    else if (M::isValidCord(params_.top) && M::isValidCord(params_.bottom))
        out->h = (ir.h - getVertValue(params_.top, ir)) - getVertValue(params_.bottom, ir);
    else
        out->h = 0;


    if (params_.ratio != 0) {
        if (out->w == 0 && out->h != 0)
            out->w = out->h * params_.ratio;
        if (out->h == 0 && out->w != 0)
            out->h = out->w / params_.ratio;
    }

    if (M::isValidCord(params_.left)) {
        out->x = getHorzValue(params_.left, ir);

        if (params_.align & M::AlignCenter)
            out->x -= out->w/2;
        else if (params_.align & M::AlignRight)
            out->x -= out->w;

    } else if (M::isValidCord(params_.right)) {
        out->x = ir.w - out->w - getHorzValue(params_.right, ir);

        if (params_.align & M::AlignCenter)
            out->x += out->w / 2;
        else if (params_.align & M::AlignLeft)
            out->x += out->w;
        //else if (params_.flags & M::AlignRight)
        //    out->x -= out->w;
    }

    if (M::isValidCord(params_.bottom)) {
        out->y = getVertValue(params_.bottom, ir);

        if (params_.align & M::AlignVCenter)
            out->y -= out->h/2;
        else if (params_.align & M::AlignTop)
            out->y -= out->h;

    } else if (M::isValidCord(params_.top)) {
        out->y = ir.h - out->h - getVertValue(params_.top, ir);

        if (params_.align & M::AlignVCenter)
            out->y += out->h/2;
        else if (params_.align & M::AlignBottom)
            out->y += out->h;
    }



    out->x += ir.x;
    out->y += ir.y;

}



} // namespace mangl
