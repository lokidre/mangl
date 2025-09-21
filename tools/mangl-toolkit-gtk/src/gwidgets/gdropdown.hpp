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

#include <apeal/signal.hpp>

#include <vector>

namespace gw {

class DropDown: public Widget {
    using Self = DropDown;
    using SelfP = Self*;

public:
    apl::Signal<int> itemSelectedSignal;


    auto create(const char* const* strings = nullptr) {
        if (strings) {
            gwidget_ = gtk_drop_down_new_from_strings(strings);
        } else {
            gwidget_ = gtk_drop_down_new(nullptr, nullptr);
        }
        //g_signal_connect(gwidget_, "activate", G_CALLBACK(Self::activate_cb), this);
        g_signal_connect(
            gwidget_, "notify::selected-item", G_CALLBACK(Self::selected_item_cb), this);
        return this;
    }

    auto create(const std::vector<apl::String>& strings) {
        std::vector<const char*> cstrings;
        for (auto& s: strings) {
            cstrings.push_back(s.c_str());
        }
        cstrings.push_back(nullptr);
        return create(cstrings.data());
    }

    auto setSelected(int idx) {
        gtk_drop_down_set_selected(GTK_DROP_DOWN(gwidget()), idx);
        return this;
    }

private:
    static void activate_cb(GtkDropDown* self, gpointer userData) {
        static_cast<SelfP>(userData)->onSignalActivate();
    }

    static void selected_item_cb(GtkDropDown* self, GParamSpec  *pspec, gpointer userData) {
        static_cast<SelfP>(userData)->onItemSelected();
    }

    void onSignalActivate() {
    }

    void onItemSelected() {
        auto idx = gtk_drop_down_get_selected(GTK_DROP_DOWN(gwidget_));
        itemSelectedSignal((int)idx);
    }
};

} // namespace gw