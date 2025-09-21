/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_app.h"

#include <mangl/effects.h>

class TestScreen: public ScreenBase {
protected:
    void onStart() override;
};

void TestScreen::onStart() {
    auto scene = App::getScene();
    auto layout = Env::getSafeArea();


    auto label = scene->newLabel({
        .text = "Press to Continue",
        .pos = layout.positioned({0.5, 0.8}).position(), //{layout.center().x, layout.center().y * 0.7},
        .color = M::White,
        .flags = M::AlignCenterAll,
    });

    scene
        ->newCrossfadeEffect({
            .widget = label,
            .color1 = M::Red,
            .color2 = M::Yellow,
            .rate = 2,
        })
        ->start();

    label = scene->newLabel({
        .text = "TAP TO CONTINUE",
        .pos = layout.positioned({0.5, 0.4}).position(),
        .color = M::White,
        .flags = M::AlignCenterAll,
    });

    auto effect = scene->newEffect<CrossfadeEffect>();
    effect->attach(label)->setColors(M::Red, M::Yellow)->setRate(0.5)->start();

    //effect->attach(label)->setColors(M::Red, M::Yellow)->setDuration(2)->setRepeat(true)->setBounce(true)->setPeriod(2)->start(); //3);
}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
