/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "gapplication.hpp"
#include "gwindow.hpp"

namespace gw {

class ApplicationWindow: public Window {
    using Self = ApplicationWindow;
    using SelfP = Self*;

    GtkApplication* gapp_{};

    static void cbDestroy(GtkWidget*, gpointer userData) {
        static_cast<SelfP>(userData)->onDestroy();
    }

public:
    ApplicationWindow() = default;

    auto create(Application* app) {
        gapp_ = app->gapp();
        gwidget_ = gtk_application_window_new(gapp_);

        g_signal_connect(gwindow(), "destroy", G_CALLBACK(Self::cbDestroy), this);

        onCreate();
    }

    auto gapp() const noexcept { return gapp_; }


protected:
    virtual void onCreate(){}
    virtual void onDestroy(){}
};

} // namespace gw
