/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <test_app.h>

#include <mangl/native.h>

class TestScreen: public Screen {
protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    buildSampleBackground();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    Size buttonSize{160, 40};
    auto button = scene->newButton({
        .text = "Show Alert...",
        .rect = safe.positioned(buttonSize, {0.5, 0.7}),
        .pushSignal = [] {
            NativeGui::alert({.title = "Alert", .message = "Sample Alert"});
        },
    });
}



void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
