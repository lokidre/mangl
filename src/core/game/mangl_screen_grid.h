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

struct GameScreenLayout {

    Rect gridContent;
    Rect gridSafe;
    Rect gridFull;

    Size gridSize;
    Rect gridCenter;

    Point adjustGrid(Point p) const noexcept {

        auto col = std::floor((p.x - gridFull.x) / gridSize.w);
        auto row = std::floor((p.y - gridFull.y) / gridSize.h);

        p.x = gridFull.x + col * gridSize.w;
        p.y = gridFull.y + row * gridSize.h;

        return p;
    }

    Point adjustGridCenter(Point p) const noexcept {
        p = adjustGrid(p);
        p.x += gridSize.w / 2;
        p.y += gridSize.h / 2;
        return p;
    }


    Rect adjustGrid(Rect r) const noexcept {
        auto p = adjustGrid(r.position());
        r.setPosition(p);
        return r;
    }

    Rect adjustGrid(Point p, Size s) const noexcept {
        p = adjustGrid(p);
        p.x -= s.w / 2;
        p.y -= s.h / 2;
        return Rect::make(p, s);
    }


    Rect adjustGridCenter(Rect r) const noexcept {
        auto p = adjustGridCenter(r.position());
        p.x -= r.w / 2;
        p.y -= r.h / 2;
        r.setPosition(p);
        return r;
    }

    Rect adjustGridCenter(Point p, Size s) const noexcept {
        p = adjustGridCenter(p);
        p.x -= s.w / 2;
        p.y -= s.h / 2;
        return Rect::make(p, s);
    }



};


}  // namespace mangl
