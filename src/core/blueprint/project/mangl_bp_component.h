/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_bp_layout.h"
#include "mangl_bp_props.h"
#include "../animation/mangl_bp_animation.h"

namespace mangl::bp {

enum class ComponentType {
    Unknown = 0,
    Rect, 
    Image, 
    Box,
    Text,
};


struct Component {
    String id;
    
    ComponentType type{};
    bool hidden{};

    PropsContainer props;

    AnimationP animation{};

    


    ComponentLayout componentLayout;
    String fillType;
    String name;
    String file;
    String text;
    //Border border;
    std::vector<Color> color;
    std::vector<Color> backgroundColor;
    String src;
    String md5;
};

using ComponentP = Component*;


}
