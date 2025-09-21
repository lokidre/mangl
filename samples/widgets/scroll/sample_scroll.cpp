/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include <mangl/widgets.h>
#include <mangl/theme.h>

namespace sample {

class TestScreen: public Screen {
protected:
    void onStart() override;
};


void TestScreen::onStart() {

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    auto center = safe.center();

    scene->newBox(Theme::body.backgroundColor.adjustedBrightness(0.3), Env::getFullArea());
    scene->newBox(Theme::body.backgroundColor, safe);

    Rect scrollRect = safe;

    scrollRect.shrinkX(6);
    scrollRect.shrinkW(6);

    scrollRect.shrinkY(10);
    scrollRect.shrinkH(10 + 30);

    scene->newRect(M::Red, scrollRect)->setWidth(3);


    PrimP prim{};

    auto scroll = scene->newWidget<ScrollWidget>()
        ->move(scrollRect)
        ->enablePaging(false)
        ->setOrientation(M::OrientationVert)->create();

    auto current = scrollRect;

    current.h = 40;
    current.y = -current.h;
    //
    //    current.shrinkY(current.h - 40);
    //
    //    //current.y -= 30;
    //

    prim = scene->newBox(M::Green, {30, current.y, 60, current.h});
    scroll->addPrim(prim);

    current.y -= current.h + 10;

    current.h = 50;
    current.y -= 40;
    current.x = 40;
    current.w = 80;

    auto button = scene->newButton({
        .text = "Button",
        .rect = current,
        });
    scroll->addWidget(button);


    current.h = 60;
    current.y -= current.h + 20;
    current.w = 100;

    prim = scene->newRect(M::Blue, current);
    scroll->addPrim(prim);



    current.y = scrollRect.y - scrollRect.h - 20;
    current.h = 40;
    current.w = 200;

    button = scene->newButton({
        .text = "Button Bottom",
        .rect = current,
        });
    scroll->addWidget(button);



    current.y -= current.h + 30;

    current.x = 100;
    current.w = 180;

    button = scene->newButton({
        .text = "Button B2",
        .rect = current,
        });
    scroll->addWidget(button);


    current.y -= current.h + 60;

    current.x = 100;
    current.w = 180;

    button = scene->newButton({
        .text = "Button B3",
        .rect = current,
        });
    scroll->addWidget(button);


    current.y -= current.h + 70;

    current.x = 100;
    current.w = 180;

    button = scene->newButton({
        .text = "Button B4",
        .rect = current,
        });
    scroll->addWidget(button);


    current.y -= current.h + 120;

    current.x = 100;
    current.w = 180;

    button = scene->newButton({
        .text = "Button B5",
        .rect = current,
        });
    scroll->addWidget(button);


}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}
