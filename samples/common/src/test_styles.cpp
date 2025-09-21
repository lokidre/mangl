/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_app.h"
#include "test_static.h"

#ifndef MANGL_CONSOLE
namespace sample {

void App::initStyles() {

    auto& sslider = M::Styles::slider;
    sslider.textures = texCommon.slider;



    auto& sscreen = M::Styles::screen;

    auto& scloseButton = sscreen.closeButton;
    scloseButton.button.texture = texCommon.closeButton;
    scloseButton.button.size = {20, 20};
    scloseButton.onPush = App::popScreen;

    auto& sbackButton = sscreen.backButton;
    sbackButton.button.texture = texCommon.backButton;
    sbackButton.button.size = {20, 20};



}

}  // namespace sample
#endif
