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
//CommonTexture texCommon{};

void TestScreen::onStart() {
    auto scene = App::getScene();

    Cord distance = 230;
    Cord radius = 20;
    Cord period = 2;
    Cord gap = radius * 2 + 20;

    Point current{80, 80};

    auto obj = scene->newPrim()->setTexture(texCommon.checkbox)->move({100,100,100,100});
//    auto rotateObj = scene->newEffect<RotateEffect>();
//    //rotateObj->attach(obj);
//    rotateObj->attachedPrim_ = obj;
//    //rotateObj->setAngle(0.25*cognit::constants::pi<Real>);
//    rotateObj->setAngle(2*cognit::constants::pi<Real>);
//    rotateObj->setPermanent(true);
//    rotateObj->setDuration(10);
//    rotateObj->setPeriod(2);
//    rotateObj->setRepeat(true);
//    rotateObj->start();

scene->newRotateEffect({
    .prim = obj,
    .angle = 2*cognit::constants::pi<Real>,
    .duration = 10,
//    .period = 2,
    .repeat = true,
})->start();

//    scene
//        ->newMoveEffect({
//            .prim = scene->newCircle(M::Red, current, radius),
//            .offset = {0, distance},
//            .duration = period,
//            .period = period,
//            .repeat = true,
//            .bounce = true,
//        })
//        ->start();

}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
