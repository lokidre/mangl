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

class Spacer: public Widget {
public:
    Spacer(Orientation orientation): orientation_{orientation} {
    }

    auto create() {
        gwidget_ = gtk_box_new(GtkOrientation(orientation_), false);

        if (orientation_ == Orientation::Vertical) {
            gtk_widget_set_vexpand(gwidget_, true);
        } else {
            gtk_widget_set_hexpand(gwidget_, true);
        }

        return this;
    }

private:
    Orientation orientation_;
};

}
