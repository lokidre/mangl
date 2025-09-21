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

    Cord distance = 130;
    Cord radius = 20;
    Cord rate = 1;
    Cord gap = radius * 2 + 20;

    Point current{80, 80};

    scene
        ->newMoveEffect({
            .prim = scene->newCircle(M::Red, current, radius),
            .offset = {0, distance},
            .rate = rate,
        })
        ->start();

    current.x += gap;

    scene
        ->newMoveEffect({
            .prim = scene->newCircle(M::Green, current, radius),
            .offset = {0, distance},
            .duration = 1/rate,
            .period = 1/rate,
            .repeat = true,
            .bounce = true,
            .easing = Easing::QuartInOut,
        })
        ->start();

    current.x += gap;

    scene
        ->newMoveEffect({
            .prim = scene->newCircle(M::Blue, current, radius),
            .offset = {0, distance},
            .rate = rate,
            .easing = Easing::CubicInOut,
        })
        ->start();

}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
