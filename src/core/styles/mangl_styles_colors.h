/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "../mangl_types.h"
#include "../mangl_color.h"

namespace mangl {

namespace ThemeColors {
enum ThemeColors: uint32_t {
    PrimaryMain = 0x90caf9,
    PrimaryLight = 0xe3f2fd,
    PrimaryDark = 0x42a5f5,
    PrimaryContrastText = 0x000000,

    InfoMain = 0x29b6f6,
    InfoLight = 0x4fc3f7,
    InfoDark = 0x0288d1,

    BackgroundPaper = 0x121212,

    TextPrimary = 0xffffff,

    //WidgetMain = 0x1976d2,

    Grey50 = 0xfafafa,
    Grey100 = 0xf5f5f5,
    Grey200 = 0xeeeeee,
    Grey300 = 0xe0e0e0,
    Grey400 = 0xbdbdbd,
    Grey500 = 0x9e9e9e,
    Grey600 = 0x757575,
    Grey700 = 0x616161,
    Grey800 = 0x424242,
    Grey900 = 0x212121,
};
}  // colors

}
