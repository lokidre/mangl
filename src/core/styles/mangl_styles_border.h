/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_styles_colors.h"

namespace mangl {

struct ThemeBorderStrike {
    Color color{};
    Cord width{1};
    Cord offset{};
};

using BorderStrikeContainer = std::vector<ThemeBorderStrike>;

struct ThemeBorder {
    Cord radius{};
    BorderStrikeContainer strikes;

    Color getColor() noexcept {
        if (strikes.empty()) return {};
        return strikes.front().color;
    }
};

}  // namespace mangl

