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
    auto layout = Env::getSafeArea();

    auto center = layout.center();

    scene->newBox(Theme::body.backgroundColor, Env::getFullArea());


    Rect rect{layout.x + 10, layout.top() - 50, 120, 40};

    auto button = scene->newButton({
        .text = "Button",
        .rect = rect,
        .pushSignal = []() { MDebugPrint("Button pressed."); },
    });

    //    rect.y -= 50;
    //
    //    scene->newButton({.text = "Button T", .rect = rect});
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}
} // namespace