/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <test_app.h>

// class AppScreen: public ScreenBase {
// protected:
//	void onStart() override ;
// } ;
using namespace mangl;
class TestScreen: public ScreenBase {
protected:
    void onStart() override;
};

void TestScreen::onStart() {
    auto scene = App::instance()->getScene();

    auto safeLayout = Env::safeLayout();
    auto fullLayout = Env::fullLayout();

    scene->newRect(M::Red, safeLayout);
}

// #include <common/framework.cpp>


//
// Initialize the layout
static bool init = [] {
    sampleApplicationSize_ = {320, 480};
    sampleApplicationLayout_ = {354, 576};
    sampleScreenPadding_ = {};
    return true;
}();

void App::onStart() {
    if (sampleApplicationLayout_.h == 0) {
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    } else {
        Env::setLayout(sampleApplicationLayout_);
    }
    switchScreen(new TestScreen);
}
