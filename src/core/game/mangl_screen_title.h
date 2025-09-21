/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_screen_title_params.h"

#include "../mangl_widgets_decl.h"


namespace mangl {

struct ScreenTitle {
    Rect rect{};  // Final title rect
    LabelP label{};
    PrimP image{};
};


}
