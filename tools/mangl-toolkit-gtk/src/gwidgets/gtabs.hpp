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

class Tabs: public Widget {
    using Self = Tabs;
    using SelfP = Self*;

public:
    apl::Signal<int> tabSelected;

    auto create() {
        gwidget_ = gtk_notebook_new();
        gtk_notebook_set_tab_pos(GTK_NOTEBOOK(gwidget_), GTK_POS_TOP);
        gtk_notebook_set_show_tabs(GTK_NOTEBOOK(gwidget_), true);
        gtk_notebook_set_show_border(GTK_NOTEBOOK(gwidget_), true);


        selSignalId_ = g_signal_connect(gwidget_, "switch-page", G_CALLBACK(switch_page_cb), this);

        //g_signal_connect(gwidget_, "delete-event", G_CALLBACK(delete_event_cb), this);
        return this;
    }

    auto setTabPosition(GtkPositionType pos) {
        gtk_notebook_set_tab_pos(GTK_NOTEBOOK(gwidget_), pos);
        return this;
    }

    auto addTab(const char* title, Widget* widget) {
        auto label = gtk_label_new(title);
        gtk_notebook_append_page(GTK_NOTEBOOK(gwidget_), widget->gwidget(), label);
        return this;
    }

    auto addTab(const String& title, Widget* widget) { return addTab(title.c_str(), widget); }

    auto setCurrentTab(int index) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(gwidget_), index);
        return this;
    }

    auto blockSignals() {
        g_signal_handler_block(gwidget_, selSignalId_);
        return this;
    }

    auto unblockSignals() {
        g_signal_handler_unblock(gwidget_, selSignalId_);
        return this;
    }

private:
    int selSignalId_{};

    static void switch_page_cb(GtkNotebook* nb, GtkWidget* page, guint pageNum, gpointer userData) {
        auto idx = gtk_notebook_get_current_page(GTK_NOTEBOOK(nb));

        // For some strange reason when the window is close "switch-page" signal is emitted
        if (idx >= 0) {
            static_cast<SelfP>(userData)->tabSelected((int)pageNum);
        }
    }

    static void destroy_cb(GtkWidget* self, gpointer user_data) { int a = 0; }
};

} // namespace gw
