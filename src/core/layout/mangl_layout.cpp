/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_layout.h"

namespace mangl {


void MLayout::update(Prim *prim, MLayoutItem &item)
{
    if (item.texture_flag) {
        if (item.textureIndex)
            prim->setTexture(atlas_->get(item.textureName, item.textureIndex));
        else
            prim->setTexture(atlas_->get(item.textureName));
    }

    if (item.size.flag) {
        prim->resize({xVal(item.size.value.w), yVal(item.size.value.h)});
    }

    if (item.position.flag) {
        Point p{};
        Size s{};

        if (item.size.flag)
            s = prim->getRect().size();
        else if (item.texture_flag) {
            s = prim->texture().size();
        }

        if (item.origin.flag)
            p = {xVal(item.origin.value.x), yVal(item.origin.value.y)};
        else
            p = {xVal(defaultOrigin_.x), yVal(defaultOrigin_.y)};

        auto axis = item.axis.flag ? item.axis.value : defaultAxis_;

        p.x += axis.x * xVal(item.position.value.x), p.y += axis.y * yVal(item.position.value.y);


        auto align = item.align.flag ? item.align.value : defaultAlign_;

        if (M::AlignCenter == align.horz)
            p.x -= s.w/2;
        else if (M::AlignRight == align.horz)
            p.x -= s.w;

        if (M::AlignVCenter == align.vert)
            p.y -= s.h/2;
        else if (M::AlignTop == align.vert)
            p.y -= s.h;

        prim->move(p);
    }


    if (item.rotation.flag) {
        prim->setRotation(item.rotation.value);
    }

    if (item.alpha.flag)
        prim->setAlpha(item.alpha.value);
}

}
