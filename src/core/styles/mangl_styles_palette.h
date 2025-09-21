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

struct ThemePaletteItem {
    Color main, light, dark, contrastText;
};

struct ThemePalette {
    ThemePaletteItem primary{
        ThemeColors::PrimaryMain,
        ThemeColors::PrimaryLight,
        ThemeColors::PrimaryDark,
        ThemeColors::PrimaryContrastText,
    };
};

}  // namespace mangl

