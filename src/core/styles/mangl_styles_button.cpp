/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_styles_button.h"

namespace mangl {

void ButtonStyle::reset() noexcept {
    clear();

    size = {100, 40};
    textSize = {16};

    textColor = ThemeColors::PrimaryContrastText;
    //textGradient = {};

    pressedTextColor = ThemeColors::PrimaryLight;
    //disabledTextColor = M::Red;
    //disabledTextColor = 0x4c4f51;
    disabledTextColor = 0x9e9e9e;


    backgroundColor = ThemeColors::PrimaryMain;
    pressedBackgroundColor = ThemeColors::PrimaryDark;
    disabledBackgroundColor = 0x2d3032;
    //disabledBackgroundColor = 0x7c7c7c;


    border = {.radius = 4};

    //texture = {};
}


}  // namespace button


