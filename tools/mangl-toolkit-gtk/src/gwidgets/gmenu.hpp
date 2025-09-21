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

namespace gw {

class MenuItem {
public:
    MenuItem(const char* label, const char* action) {
        if (label && action) {
            gitem_ = g_menu_item_new(label, action);
        }
    }

    auto create(const char* label, const char* action) {
        if (gitem_)
            return this;
        gitem_ = g_menu_item_new(label, action);
        return this;
    }

//    auto setSection(bool flag) {
//        return this;
//    }


    GMenuItem* gitem() const noexcept { return gitem_; }

private:
    GMenuItem* gitem_{};
};


class Menu {
public:

    auto create() {
        gmenu_ = g_menu_new();
        return this;
    }

    auto addSubMenu(const char* label, Menu* submenu) {
        g_menu_append_submenu(gmenu_, label, G_MENU_MODEL(submenu->gmenu()));
        return this;
    }

    //auto addItem(const char *label, const char* action) {
    auto addItem(const MenuItem& item) {
//        auto item = g_menu_item_new(label, action);
        g_menu_append_item(gmenu_, item.gitem());
        return this;
    }

    auto addSeparator() {
        //auto separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        //auto separator = g_menu_s
        //g_menu_append_item(gmenu_, separator);
        return this;
    }



    auto setGMenu(GObject* gmenu) {
        gmenu_ = (GMenu*)gmenu;
        return this;
    }

    GMenu* gmenu() const noexcept { return gmenu_; }

private:
    GMenu* gmenu_{};
};




}
