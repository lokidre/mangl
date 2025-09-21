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

#include <apeal/geo.hpp>

namespace gw {

class Window: public Widget {

public:
    auto setTitle(const char* title) {
        gtk_window_set_title(GTK_WINDOW(gwidget_), title);
        return this;
    }

    auto setDetaultSize(int width, int height) {
        gtk_window_set_default_size(GTK_WINDOW(gwidget_), width, height);
        return this;
    }

    SizeI getDefaultSize() {
        int width{}, height{};
        gtk_window_get_default_size(GTK_WINDOW(gwidget_), &width, &height);
        return {width, height};
    }

    auto setShowMenubar(bool flag) {
        gtk_application_window_set_show_menubar(GTK_APPLICATION_WINDOW(gwindow()), flag);
        return this;
    }


    auto setChild(Widget* widget) {
        gtk_window_set_child(GTK_WINDOW(gwidget_), widget->gwidget());
        return this;
    }

    auto preset() {
        gtk_window_present(gwindow());
        return this;
    }


    GtkWindow* gwindow() const noexcept { return (GtkWindow*)gwidget(); }

};


} // namespace gw
