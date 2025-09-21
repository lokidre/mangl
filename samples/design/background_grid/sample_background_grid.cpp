/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include <mangl/design.h>

namespace sample {

class TestScreen: public Screen {

    DesignGridWidgetP buildLayoutGrid();
    DesignGridWidgetP buildVerticesGrid();

protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    // buildSampleBackground();

    buildVerticesGrid();

    // buildLayoutGrid();
}

DesignGridWidgetP TestScreen::buildVerticesGrid() {
    return App::getScene()->newDesignGridWidget({
        .fullscreen = true,
        .gridDims = {.w = 10},
        //.gridSize = {.w = 30},
        .vertexType = PrimType::Disk,

        .vertexWidthRange = {.mn = 3, .mx = 30},

        .vertexColor = M::SkyBlue,
    });
}


DesignGridWidgetP TestScreen::buildLayoutGrid() {
    auto scene = App::getScene();

    Rect rect{20, 40, 200, 390};

    auto widget = scene->newDesignGridWidget({
        //.fullscreen = true,
        .rect = rect,
        .gridDims = {.w = 10},
        //.gridSize = {.w = 30},
        .vertexType = PrimType::Disk,
        .vertexColor = M::Blue,
    });

    scene->newRect(M::Red, rect);

    return widget;
}


void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} // namespace 
