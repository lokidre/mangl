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

class CheckBox: public Widget {
    using Self = CheckBox;
    using SelfP = Self*;

public:
    apl::Signal<bool> onToggle;

    explicit CheckBox(const char* label = nullptr): label_{label} {}

    auto create(const char* label = nullptr) {
        if (label)
            label_ = label;

        if (label_) {
            gwidget_ = gtk_check_button_new_with_label(label_);
        } else {
            gwidget_ = gtk_check_button_new();
        }

        gtk_check_button_set_active(GTK_CHECK_BUTTON(gwidget_), initChecked_);

        g_signal_connect(gwidget_, "toggled", G_CALLBACK(toggled_cb), this);

        return this;
    }

    auto setChecked(bool flag) {
        initChecked_ = flag;
        if (gwidget_) {
            gtk_check_button_set_active(GTK_CHECK_BUTTON(gwidget_), flag);
        }
        return this;
    }

private:
    const char* label_{};
    bool initChecked_{};

    static void toggled_cb(GtkCheckButton* self, gpointer userData) {
        auto active = gtk_check_button_get_active(self);
        static_cast<SelfP>(userData)->onToggle(active);
    }
};

} // namespace gw
