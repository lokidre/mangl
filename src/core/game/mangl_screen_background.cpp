/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_game_screen.h"

#include "../mangl_scene.h"

namespace mangl {

void GameScreenBase::buildBackground(ScreenType type) {

    const auto& sstyle = M::Styles::screen;
    const auto& sbackground = sstyle.background;

    onBuildBackground(type);

    if (sbackground.onBuild) {
        sbackground.onBuild(type);
    } else {
        if (sbackground.color)
            mainScene_->newBox(sbackground.color, fullArea_);
    }
}


}  // namespace mangl


