/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "gwidgets/widgets.hpp"

#include "state.hpp"

namespace toolkit {

class FileSelectorWidget {
    using Self = FileSelectorWidget;

public:
    void create();

    GtkWidget* gwidget() { return filesBox_.gwidget(); }

private:
    gw::VBoxLayout filesBox_;

    gw::Tabs projectTabs_;
    //gw::Frame projectTabWidgets_[(int)ProjectCategory::Count];
    gw::ListView projectTabListViews_[(int)ProjectCategory::Count];


    gw::ActionBar actionBar_;
    gw::Button actionButtonAdd_{nullptr, "list-add-symbolic"};
    gw::FileChooserDialog fileAddDialog_;

    gw::Button actionButtonRemove_{nullptr, "list-remove-symbolic"};
};

}  // namespace toolkit
