/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_styles_border.h"

#include "../mangl_texture.h"


namespace mangl {

struct ButtonStyle {
    Size size; //{100, 40};
    Cord textSize; //{16};

    Color textColor; //{ThemeColors::PrimaryContrastText};
    ColorGradient textGradient;
    Color pressedTextColor; //{ThemeColors::PrimaryLight};
    //Color disabledTextColor{M::Red};
    // Color disabledTextColor{0x4c4f51};
    Color disabledTextColor; //{0x9e9e9e};

    Color backgroundColor; // {ThemeColors::PrimaryMain};
    Color pressedBackgroundColor; //{ThemeColors::PrimaryDark};
    Color disabledBackgroundColor; //{0x2d3032};
    // Color disabledBackgroundColor{0x7c7c7c};

    ThemeBorder border; //{.radius = 4};
    Color getBorderColor() noexcept { return border.getColor(); }


    Texture texture;

    void clear() noexcept {
        ButtonStyle s{};
        std::swap(*this, s);
    }

    void reset() noexcept;

};

}  // namespace mangl

