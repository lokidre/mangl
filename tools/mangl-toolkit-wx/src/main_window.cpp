/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "main_window.h"

#include <wx/msgdlg.h>

void MainWindow::init()
{
    createMainMenu();
    CreateStatusBar();
    SetStatusText("Ready.");
}


void MainWindow::createMainMenu() {
    auto menuBar = new wxMenuBar();

    auto menu = new wxMenu();

    menu->Append(IdHello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");

    menu->AppendSeparator();

    menu->Append(wxID_EXIT);
    Bind(wxEVT_MENU, [this](auto&) { Close(true); }, wxID_EXIT);


    menuBar->Append(menu, "&File");


    menu = new wxMenu();

    menu->Append(wxID_ABOUT);
    Bind(wxEVT_MENU, [](auto&) {
        wxMessageBox("Mangl Toolkit", "About Mangl Toolkit", wxOK | wxICON_INFORMATION);
    }, wxID_ABOUT);

    menuBar->Append(menu, "&Help");


    SetMenuBar(menuBar);

}
