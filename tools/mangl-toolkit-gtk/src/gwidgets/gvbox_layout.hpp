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

class VBoxLayout: public BoxLayout {
private:
    using Self = VBoxLayout;

public:
    auto create() {
        create_(GTK_ORIENTATION_VERTICAL, false);
        return this;
    }

    //    Self& gappend(void* gwidget) {
    //        gtk_box_append(gwidget_, GTK_WIDGET(gwidget));
    //        return *this;
    //    }
};

} // namespace gw
