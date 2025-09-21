/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "central_widget.hpp"
#include "gwidgets/widgets.hpp"

namespace toolkit {

class MainWindow: public gw::ApplicationWindow {

    gw::Menu mainMenu_;

    void createMainMenu();
    //void createToolbar();
    void createCentralWidget();
    // void createStatusbar();

    gw::VBoxLayout mainLayout_;
    //GtkBox* mainLayout_{};
    //GtkStatusbar* statusbar_{};
    gw::StatusBar statusBar_;

    CentralWidget centralWidget_;


protected:
    void onCreate() override;
    void onDestroy() override;
};

}  // namespace toolkit
