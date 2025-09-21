/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/mangl.h>

#include "mangl_bp_coord.h"


namespace mangl::bp {

enum class ScreenOrientation {
    Portrait,
    Landscape,
};

enum class ScreenConstraint {
    Width,
    Height,
};


struct ScreenLayout {
    Cord width{};
    Cord height{};
    ScreenOrientation orientation;
    ScreenConstraint constraint;
};


struct ComponentLayout {
    CoordMajor left;
    CoordMajor right;
    CoordMajor top;
    CoordMajor bottom;
    CoordMajor width;
    CoordMajor height;

    int alignHorz{};
    int alignVert{};
    Cord x{};
    Cord y{};
};
   

}

