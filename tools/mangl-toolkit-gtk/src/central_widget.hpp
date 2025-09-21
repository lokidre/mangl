/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <main_types.hpp>

#include "gwidgets/widgets.hpp"

#include "game_screen_widget.h"
#include "properties_widget.h"
#include "file_selector_widget.h"

namespace toolkit {

class CentralWidget: public gw::Frame {

    gw::HBoxLayout layout_;

    FileSelectorWidget fileSelectorWidget_;
    GameScreenWidget gameScreenWidget_;
    PropertiesWidget propertiesWidget_;

protected:
    void onCreate() override {
        setHExpand()->setVExpand();

        setChild(layout_.create());

        fileSelectorWidget_.create();
        layout_.gappend(fileSelectorWidget_.gwidget());

        gameScreenWidget_.create();
        layout_.gappend(gameScreenWidget_.gwidget());

        layout_.append(propertiesWidget_.create("Properties"));
        //propertiesWidget_.create();
        //layout_.gappend(propertiesWidget_.gwidget());
    }
};

}  // namespace toolkit
