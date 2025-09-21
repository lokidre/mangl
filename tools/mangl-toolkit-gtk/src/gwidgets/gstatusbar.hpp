/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "gwidget.hpp"

namespace gw {

class StatusBar: public Widget {
public:
    auto create() {
        // gwidget_ = gtk_statusbar_new();
        return this;
    }

    auto push(const char* message) {
        // gtk_statusbar_push(GTK_STATUSBAR(gwidget_), 1, "Ready.");
        return this;
    }
};

}  // namespace gw
