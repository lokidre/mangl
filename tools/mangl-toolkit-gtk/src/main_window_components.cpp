/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "main_window.h"

#include "main_application.hpp"
#include "main_data.h"
#include <apeal/string.hpp>

namespace toolkit {

static void activate_action(GSimpleAction* action, GVariant* param, gpointer userData) {

    const char* val = "";

    if (param) {
        val = g_variant_get_string(param, nullptr);
    }

    g_print("Action %s activated: %s\n", g_action_get_name(G_ACTION(action)), val);
}

static void activate_radio(GSimpleAction* action, GVariant* param, gpointer userData) {

    const char* val = "";

    if (param) {
        val = g_variant_get_string(param, nullptr);
    }

    g_print("Action %s activated: %s\n", g_action_get_name(G_ACTION(action)), val);

    //g_action_change_state(G_ACTION(action), param);
    g_simple_action_set_state(G_SIMPLE_ACTION(action), param);
}


constexpr GActionEntry entries[] = {
    //{"app.quit", activate_action, NULL, NULL, NULL, {0, 0, 0}},
    {"open", activate_action},
    {"quit", activate_action},

    {"orientation", activate_radio, "s", "'vertical'"},
    {"zoom", activate_radio, "s", "'100'"},
};

void MainWindow::createMainMenu() {

    mainMenu_.create();
    auto menubar = mainMenu_.gmenu();

    //auto menubar = g_menu_new();

    //
    // File menu
    //
    GMenuItem *menu_item_menu = g_menu_item_new ("Menu", NULL);
    GMenu *menu = g_menu_new();
    GMenuItem *menu_item_quit = g_menu_item_new ("Quit", "app.quit");
    g_menu_append_item (menu, menu_item_quit);
    g_menu_item_set_submenu (menu_item_menu, G_MENU_MODEL(menu));
    g_menu_append_item (menubar, menu_item_menu);

    // auto file_menu = g_menu_new();
    // g_menu_append(file_menu, "Quit", "app.quit");
    // g_menu_append_submenu(mainMenu_.gmenu(), "File", G_MENU_MODEL(file_menu));

    // //
    // // Help menu
    // //
    // GMenu *help_menu = g_menu_new();
    // g_menu_append(help_menu, "About", "app.about");
    // g_menu_append_submenu(mainMenu_.gmenu(), "Help", G_MENU_MODEL(help_menu));

    //GtkWidget *menu_bar = gtk_popover_menu_bar_new_from_model(G_MENU_MODEL(mainMenu_.gmenu()));



    /*

//        auto action = g_simple_action_new("app.quit", NULL);
//        g_action_map_add_action (G_ACTION_MAP (gapp()), G_ACTION (action));
//        g_signal_connect(action, "activate", G_CALLBACK(activate_action), NULL);

    auto actionMap = G_ACTION_MAP(gapp());
    g_action_map_add_action_entries(actionMap, entries, G_N_ELEMENTS(entries), nullptr);

    auto action = g_action_map_lookup_action(actionMap, "orientation");

    //g_simple_action_set_state(G_SIMPLE_ACTION(action), g_variant_new_string("'vertical'"));
    //g_simple_action_set_enabled(G_SIMPLE_ACTION(action), false);

//    action = g_action_map_lookup_action(actionMap, "zoom");
//    g_simple_action_set_state(G_SIMPLE_ACTION(action), g_variant_new_string("'100'"));
    //g_simple_action_set_enabled(G_SIMPLE_ACTION(action), false);


    auto menusBuilder = gtk_builder_new_from_resource("/ui/main-menu.xml");
    auto menu = gtk_builder_get_object(menusBuilder, "menubar");
    mainMenu_.setGMenu(menu);
*/

    MainApplication::instance()->setMenubar(&mainMenu_);
    setShowMenubar(true);
}


void MainWindow::createCentralWidget() {
    centralWidget_.create();
    mainLayout_.append(&centralWidget_);
}

// void MainWindow::createStatusbar() {
//     mainLayout_.append(statusBar_.create());
//     statusBar_.push("Ready.");
// }

}  // namespace toolkit
