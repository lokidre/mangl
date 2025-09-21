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

    buildSampleBackground();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    auto center = safe.center();

    Size blockSize{20, 20};
    Cord cornerRadius = 11;


    //
    // Four corners and borders
    //
    auto rect = safe.positioned({120, 80}, {0.5, 0.8});

    scene->newBackgroundWidget({
        .color = M::DodgerBlue,

        .cornerTexture = texCommon.buttonCorners,
        .cornerRadius = cornerRadius,

        .borderTexture = texCommon.buttonBorders,

        .blockSize = blockSize,
        .rect = rect,
    });


    //
    // Top left corner and borders
    //
    rect = safe.positioned({160, 120}, {0.5, 0.6});

    scene->newBackgroundWidget({
        .color = M::Blue,

        .cornerLeftTopTexture = texCommon.buttonCorners.getFrame(0),
        .cornerRadius = cornerRadius,

        .borderTexture = texCommon.buttonBorders,

        .blockSize = blockSize,
        .rect = rect,
    });


    //
    // Left fill right textures
    //

    rect = safe.positioned({200, 40}, {0.5, 0.4});

    scene->newBackgroundWidget({
        .texture = texCommon.infoBlock.getFrame(1),
        .beginTexture = texCommon.infoBlock.getFrame(0),
        .endTexture = texCommon.infoBlock.getFrame(2),
        .blockSize = {texCommon.infoBlock.ratioWidth(rect.h), rect.h},
        .rect = rect,
    });
}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} // namespace sample
