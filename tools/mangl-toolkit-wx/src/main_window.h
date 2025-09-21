/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include <wx/frame.h>
#include <wx/menu.h>

enum {
    IdHello = 1,
};

class MainWindow: public wxFrame {
public:
    MainWindow(): wxFrame(nullptr, wxID_ANY, "Mangl Toolkit"){}
    void init();

private:
    void createMainMenu();
};
