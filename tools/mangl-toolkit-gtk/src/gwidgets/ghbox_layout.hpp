/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "gbox_layout.hpp"

namespace gw {

class HBoxLayout: public BoxLayout {
private:
    using Self = HBoxLayout;

public:
    auto create() {
        create_(GTK_ORIENTATION_HORIZONTAL, false);
        return this;
    }
};

}
