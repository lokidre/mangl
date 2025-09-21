/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "gobject.hpp"

namespace gw {

class Widget: public Object {

public:
    GtkWidget* gwidget() const noexcept { return gwidget_; }

    auto setVExpand(bool flag = true) {
        gtk_widget_set_vexpand(gwidget_, flag);
        return this;
    }

    auto setHExpand(bool flag = true) {
        gtk_widget_set_hexpand(gwidget_, flag);
        return this;
    }

    auto setSizeRequest(int width, int height) {
        gtk_widget_set_size_request(gwidget_, width, height);
        return this;
    }

    auto setEnabled(bool flag = true) {
        gtk_widget_set_sensitive(gwidget_, flag);
        return this;
    }

    auto setName(const char* name) {
        gtk_widget_set_name(gwidget_, name);
        return this;
    }

    auto setMargin(int margin) {
        gtk_widget_set_margin_top(gwidget_, margin);
        gtk_widget_set_margin_end(gwidget_, margin);
        gtk_widget_set_margin_bottom(gwidget_, margin);
        gtk_widget_set_margin_start(gwidget_, margin);
        return this;
    }


protected:
    GtkWidget* gwidget_{};
};

} // namespace gw
