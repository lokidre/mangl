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
    Cord period = 1;
    Cord gap = radius * 2 + 20;

    Point current{80, 80};

    auto prim = scene->newCircle(M::Red, current, radius);

//    scene->newEffect<ScaleEffect>()->setScale(2)->attach(prim)->setBounce(true)
//            ->setRepeat(true)->start();

    scene->newScaleEffect({
        .prim = prim,
        .scale = 1.5,
        .period = period,
        .repeat = true,
        .bounce = true,


    })->start();

    //TODO what do these do?
    //setRate(Timestamp)
    //setEasing(Timestamp)
    //setDisappear
    //setPermanent(true)
//    scene
//            ->newMoveEffect({
//                                    .prim = scene->newCircle(M::Red, current, radius),
//                                    .offset = {0, distance},
//                                    .duration = period,
//                                    .period = period,
//                                    .repeat = true,
//                                    .bounce = true,
//                            })
//            ->start();

}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
