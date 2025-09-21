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

    auto center = layout.center();
    Point current = {center.x, layout.top() - 60};

    Cord gap = 50;

    auto label = scene->newLabel({
        .text = "Fade Out",
        .pos = current,
        .flags = M::AlignCenterAll,
    });

    scene
        ->newFadeEffect({
            .widget = label,
            .duration = 4,
            .permanent = true,
        })
        ->start();

    current.y -= gap;


    label = scene->newLabel({
        .text = "Periodic",
        .pos = current,
        .flags = M::AlignCenterAll,
    });

    scene
        ->newFadeEffect({
            .widget = label,
            .rate = 0.5,
        })
        ->start();


    current.y -= gap;


    label = scene
                ->newLabel({
                    .text = "Fade In",
                    .pos = current,
                    .transparent = true,
                    .flags = M::AlignCenterAll,
                });
                //->setAlpha(0);

    scene
        ->newFadeEffect({
            .widget = label,
            .alpha = 1,
            .duration = 2,
            .permanent = true,
        })
        ->start(2);

}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
