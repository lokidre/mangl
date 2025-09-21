/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../styles/mangl_styles_colors.h"
#include "../styles/mangl_styles_palette.h"
#include "mangl_game_screen_types.h"

namespace mangl {

struct GameScreenBackgroundStyles {
    //Color color{ThemeColors::Grey800};
    Color color{};

    using Callback = std::function<void(GameScreenType)>;

    Callback onBuild{};

};

}  // namespace mangl

