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

    Cord distance = 230;
    Cord radius = 20;
    Cord period = 2;
    Cord gap = radius * 2 + 20;

    Point current{80, 80};

    auto circlePrim = scene->newCircle(M::Red, current, radius);
    auto moveEffect =scene
        ->newMoveEffect({
            .prim = circlePrim,
            .offset = {0, distance},
            .duration = period,
            .period = period,
            .repeat = true,
            .bounce = true,
        });
//    auto disEffect = scene->newEffect<DisappearEffect>();
//    disEffect->attach(circlePrim);
//    disEffect->setPermanent(true);
//    disEffect->setDuration(3);

    auto disEffect = scene->newDisappearEffect({
        .prim = circlePrim,
        .duration = 3,
        .permanent = true,
    });
    moveEffect->start();
    disEffect->start();
}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
