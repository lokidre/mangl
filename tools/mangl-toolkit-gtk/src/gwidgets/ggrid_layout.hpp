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

class GridLayout: public Widget {
public:
    auto create() {
        gwidget_ = gtk_grid_new();
        return this;
    }

    auto setColumnSpacing(int spacing) {
        gtk_grid_set_column_spacing(GTK_GRID(gwidget_), (guint)spacing);
        return this;
    }

    auto setRowSpacing(int spacing) {
        gtk_grid_set_row_spacing(GTK_GRID(gwidget_), (guint)spacing);
        return this;
    }

    auto setSpacing(int horz, int vert) {
        setColumnSpacing(horz);
        setRowSpacing(vert);
        return this;
    }

    auto gattach(GtkWidget* child, int col, int row, int width = 1, int height = 1) {
        gtk_grid_attach(GTK_GRID(gwidget_), child, col, row, width, height);
        return this;
    }

    auto attach(Widget* child, int col, int row, int width = 1, int height = 1) {
        return gattach(child->gwidget(), col, row, width, height);
    }


private:
};

} // namespace gw
