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

class BoxLayout: public Widget {
private:
    using Self = BoxLayout;

public:
    auto gappend(void* widget) {
        gtk_box_append(GTK_BOX(gwidget_), GTK_WIDGET(widget));
        return this;
    }

    auto append(Widget* widget) {
        return gappend(widget->gwidget());
    }

    auto setSpacing(int spacing) {
        gtk_box_set_spacing(GTK_BOX(gwidget_), spacing);
        return this;
    }


protected:
    auto create_(GtkOrientation orientation, bool spacing) {
        gwidget_ = gtk_box_new(orientation, false);
        return this;
    }

};

}
