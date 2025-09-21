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


class ListView: public Widget {
    using Self = ListView;
    using SelfP = Self*;

public:
    apl::Signal<int> itemSelected;

private:
    SelectionMode selectionMode_{SelectionMode::SingleSelection};

public:
    auto setSelectionMode(SelectionMode mode) {
        selectionMode_ = mode;
        if (gwidget_) {
            updateSelectionMode();
            gtk_list_view_set_model(GTK_LIST_VIEW(gwidget_), gSelectionModel_);
        }
        return this;
    }

    auto create() {

        auto factory = gtk_signal_list_item_factory_new();
        g_signal_connect(factory, "setup", G_CALLBACK(setup_cb), this);
        g_signal_connect(factory, "bind", G_CALLBACK(bind_cb), this);
        // g_signal_connect (factory, "unbind", G_CALLBACK (unbind_cb), this);
        // g_signal_connect (factory, "teardown", G_CALLBACK (teardown_cb), this);


        stringList_ = gtk_string_list_new(nullptr);
        updateSelectionMode();

        gwidget_ = gtk_list_view_new(GTK_SELECTION_MODEL(gSelectionModel_), factory);

        gtk_list_view_set_single_click_activate(GTK_LIST_VIEW(gwidget_), true);

        activateId_ = g_signal_connect(gwidget_, "activate", G_CALLBACK(activate_cb), this);
        return this;
    }

    auto addItem(const char* label) {
        gtk_string_list_append(stringList_, label);
        return this;
    }

    auto addItem(const String& label) {
        return addItem(label.c_str());
    }

    auto selectItem(int index) {
        switch (selectionMode_) {
        case SelectionMode::SingleSelection:
            gtk_single_selection_set_selected(GTK_SINGLE_SELECTION(gSelectionModel_), index);
            break;
        default: break;
        }

        return this;
    }

    auto clearItems() {
        auto count = g_list_model_get_n_items(G_LIST_MODEL(stringList_));
        if (count) {
            gtk_string_list_splice(stringList_, 0, count, nullptr);
        }
    }

    auto blockSignals() {
        g_signal_handler_block(gwidget_, activateId_);
        return this;
    }

    auto unblockSignals() {
        g_signal_handler_unblock(gwidget_, activateId_);
        return this;
    }


private:
    GtkStringList* stringList_{};
    GtkSelectionModel* gSelectionModel_{};
    int activateId_{};

    static void setup_cb(GtkSignalListItemFactory*, GtkListItem* listItem, gpointer userData) {
        auto lb = gtk_label_new(nullptr);
        gtk_label_set_xalign(GTK_LABEL(lb), 0);
        gtk_list_item_set_child(listItem, lb);
    }

    static void bind_cb(GtkSignalListItemFactory*, GtkListItem* listItem, gpointer userData) {
        auto lb = gtk_list_item_get_child(listItem);
        auto strobj = (GtkStringObject*)gtk_list_item_get_item(listItem);
        gtk_label_set_text(GTK_LABEL(lb), gtk_string_object_get_string(strobj));
    }

    static void activate_cb(GtkListView*, guint position, gpointer userData) {
        static_cast<SelfP>(userData)->itemSelected((int)position);
    }

    void updateSelectionMode() {
        if (gSelectionModel_) {
            g_object_unref(gSelectionModel_);
            gSelectionModel_ = nullptr;
        }

        switch (selectionMode_) {
        case SelectionMode::NoSelection: {
            auto ns = gtk_no_selection_new(G_LIST_MODEL(stringList_));
            gSelectionModel_ = (GtkSelectionModel*)ns;
            break;
        }
        case SelectionMode::SingleSelection: {
            auto ss = gtk_single_selection_new(G_LIST_MODEL(stringList_));
            gtk_single_selection_set_autoselect(ss, false);
            //gtk_single_selection_set_can_unselect(ss, true);
            gSelectionModel_ = (GtkSelectionModel*)ss;
            break;
        }
        default: break;
        }
    }
};

} // namespace gw
