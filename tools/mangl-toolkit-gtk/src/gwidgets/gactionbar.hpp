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

class ActionBar: public Widget {
public:
    auto create() {
        gwidget_ = gtk_action_bar_new();
        return this;
    }

    auto gpackEnd(GtkWidget* widget) {
        gtk_action_bar_pack_end(GTK_ACTION_BAR(gwidget_), widget);
        return this;
    }

    auto packEnd(Widget* widget) {
        return gpackEnd(widget->gwidget());
    }

};


} // namespace gw
