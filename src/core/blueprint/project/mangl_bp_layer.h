/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_bp_component.h"
#include "../animation/mangl_bp_animation.h"

namespace mangl::bp {

struct Layer: apl::NonCopyable {
    String id;
    String name;
    bool hidden{};

    std::vector<Component*> components;

    AnimationP animation{};
    
    ~Layer() noexcept {
        clear();
    }

    void clear() noexcept {
        if (animation) {
            delete animation;
            animation = nullptr;
        }
    }
    
};

using LayerP = Layer*;


}
