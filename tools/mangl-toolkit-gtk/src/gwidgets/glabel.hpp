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

class Label: public Widget {
public:
    Label(const char* label = nullptr) {
        if (label)
            gwidget_ = gtk_label_new(label);
    }

    auto create(const char* label) {
        gwidget_ = gtk_label_new(label);
        return this;
    }

    auto setXAlign(float align) {
        gtk_label_set_xalign(GTK_LABEL(gwidget_), align);
        return this;
    }


};

}
