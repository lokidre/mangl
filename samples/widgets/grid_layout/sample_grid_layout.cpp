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


    Rect rect{layout.x + 120, layout.top() - 50, 40, 40};

    auto grid = scene->newGridLayout({
        .rect = {10, 100, layout.w - 20, layout.h - 200},
    });

    grid->addItem({
        .pos = {0, 0},
        .prim = scene->newBox(M::Red, {}),
    });

    grid->addItem({
        .pos = {1, 0},
        .widget = scene->newLabel({
            .text = "Label 1",
        }),
    });

    grid->addItem({
        .pos = {0, 1},
        .prim = scene->newBox(M::Blue, {}),
    });
    grid->addItem({
        .pos = {1, 1},
        .prim = scene->newBox(M::Magenta, {}),
    });

    grid->addItem({
        .pos = {0, 2},
        .widget = scene->newLabel({
            .text = "Align V Center",
            .textSize = 10,
            .flags = M::AlignVCenter,
        }),
    });

    grid->addItem({
        .pos = {1, 2},
        .widget = scene->newLabel({
            .text = "Align Center Right",
            .textSize = 10,
            .flags = M::AlignVCenter | M::AlignRight,
        }),
    });

    grid->addItem({
        .pos = {0, 3},
        .prim = scene->newBox(M::Yellow, {}),
    });
    grid->addItem({
        .pos = {1, 3},
        .prim = scene->newBox(M::Crimson, {}),
    });
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} // namespace