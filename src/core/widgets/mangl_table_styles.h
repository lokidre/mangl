/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "../styles/mangl_styles_palette.h"
#include "../styles/mangl_styles_separator.h"
#include "../styles/mangl_styles_text.h"

namespace mangl {

struct TableStyles {
    Color backgroundColor{ThemeColors::Grey900, 0.8};

    SeparatorStyles horzBorder;
    SeparatorStyles vertBorder;

    SeparatorStyles horzSep{
        .color =  ThemeColors::Grey700,
        .width = 1,
    };

    SeparatorStyles vertSep{
        .color =  ThemeColors::Grey700,
        .width = 1,
    };

    Cord rowHeight{16};
    Padding cellPadding{.h = 4, .v = 2};
    TextStyle contentText{
        .color =  ThemeColors::InfoMain,
        .size = 11,
    };
};


}  // namespace mangl
