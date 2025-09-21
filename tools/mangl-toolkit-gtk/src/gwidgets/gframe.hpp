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

class Frame: public Widget {
    using Self = Frame;
    using SelfP = Self*;

public:
    auto create(const char* label = nullptr) {
        gwidget_ = gtk_frame_new(label);
        onCreate();
        return this;
    }

    auto setChild(Widget* widget) {
        gtk_frame_set_child(GTK_FRAME(gwidget()), widget->gwidget());
        return this;
    }

protected:
    virtual void onCreate() {}
};

} // namespace gw
