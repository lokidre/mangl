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

namespace gw {

class Button: public Widget {
    using Self = Button;
    using SelfP = Button*;

public:
    apl::Signal<> clicked;


    Button(const char* label, const char* image = nullptr): label_{label}, image_{image} {}

    auto create() {
        if (label_) {
            gwidget_ = gtk_button_new_with_label(label_);
        } else if (image_) {
            gwidget_ = gtk_button_new_from_icon_name(image_);
        }

        g_signal_connect(gwidget_, "clicked", G_CALLBACK(Self::clicked_cb), this);
        return this;
    }

    //    template <class C>
    //    //auto connect(typename C::Func&& f, C* c) {
    //    auto connect(typename apl::impl::SlotItem<C>::M&& m, C* c) {
    //        clicked.connect(m, c);
    //        return this;
    //    }

private:
    char const* label_;
    char const* image_;

    static void clicked_cb(GtkButton* self, gpointer userData) {
        static_cast<SelfP>(userData)->clicked();
    }
};

} // namespace gw
