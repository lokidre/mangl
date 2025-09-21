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
#include "gmenu.hpp"

namespace gw {

class Application: public Object {
    using Self = Application;
    using SelfP = Self*;

    GtkApplication* gapp_{};

    static void cbActivate(GtkApplication* self, gpointer userData) {
        static_cast<SelfP>(userData)->onActivate();
    }


public:
    int run(int argc, char** argv) {
        gapp_ = gtk_application_new(app_id_, G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(gapp_, "activate", G_CALLBACK(cbActivate), this);
        auto status = g_application_run(G_APPLICATION(gapp_), argc, argv);
        g_object_unref(gapp_);
        return status;
    }

    [[nodiscard]] auto gapp() const noexcept { return gapp_; }

    auto setMenubar(Menu* menu) {
        gtk_application_set_menubar(gapp(), G_MENU_MODEL(menu->gmenu()));
    }

protected:
    virtual void onActivate() {}

    char const* app_id_{};
};

} // namespace gw
