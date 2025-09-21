/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "main_window.h"
#include "gwidgets/widgets.hpp"

#include <apeal/singleton.hpp>

#include <gtk/gtk.h>
#include <memory>

// #include "generated/resource_styles.h"

#include <mangl/environment.h>

namespace toolkit {

class MainApplication: public gw::Application, public apl::Singleton<MainApplication> {
public:
    MainWindow mainWindow;


    void onActivate() override {
        Env::instance();

        auto provider = gtk_css_provider_new();

//        auto resources = resources_get_resource();
//        GError* error{};
//        auto data = g_resource_lookup_data(
//            resources, "/ui/theme.css", G_RESOURCE_LOOKUP_FLAGS_NONE, &error
//        );
//        auto css = (const char*)g_bytes_get_data(data, 0);

//        gtk_css_provider_load_from_data(provider, css, -1);

        //gtk_css_provider_load_from_string(provider, resourceTheme_);

        auto display = gdk_display_get_default();
        gtk_style_context_add_provider_for_display(
            display,
            GTK_STYLE_PROVIDER(provider),
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
        g_object_unref(provider);

        mainWindow.create(this);

//        mainWindow_ = std::make_unique<MainWindow>(gapp());
//        mainWindow_->create();


        /*
        auto appRect = Env::applicationRect.convert<RectI>();

        if (appRect.w == Env::InvalidCoord || appRect.h == Env::InvalidCoord) {
            appRect.resize(Env::DefaultAppSize.convert<SizeI>());
        }

        auto screenSize = Env::physScreenSize.convert<SizeI>();
        auto windowSize = SizeI::make(appRect.w, appRect.h);

        auto desktopRect = RectI::make(0, 0, screenSize.w, screenSize.h);

        auto wndRect = desktopRect.centered(windowSize);

        if (appRect.x == Env::InvalidCoordI || appRect.y == Env::InvalidCoordI) {
            appRect.x = wndRect.x;
            appRect.y = wndRect.y;
        }

        Env::applicationRect = appRect.convert<Rect>();


        gtk_window_set_default_size(gwindow, wndRect.w, wndRect.h);
         */

    }

public:
    //std::unique_ptr<MainWindow> mainWindow_;
};

}  // namespace toolkit
