/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../core/mangl_setup.h"

#if MANGL_GTK

#include <gtk/gtk.h>

#include "../core/mangl_environment.h"

#include "mangl_gtk_main_window.h"

static void activate_cb(GtkApplication* app) {
    mangl::ManglMainWindow::instance()->create(app);
}

#if MANGL_NO_MAIN

#else

int main(int argc, char* argv[]) {
    gtk_init();

    // Create a new application
    auto app = gtk_application_new("com.mangl.app", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate_cb), NULL);
    auto status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}

#endif // int main()

#endif // GTK
