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


void TestScreen::onStart() 
{

    buildSampleBackground();

    auto scene = App::getScene();
    auto layout = Env::getSafeArea();

    buildBackground();

    Size segmentedSize{280, 40};
    SegmentedP segmented{};

    segmented = scene->newSegmented({
        .rect = layout.positioned(segmentedSize, {0.5, 0.8}),
        .segments = {
            {
                .label = "Left",
                .texture = texCommon.segmentedButtonFirst,
            },
        },
    });

    // segmented->addSegment({
    //     .text = "Left",
    //     .segmentTex = texCommon.segmentedButtonFirst,
    // });

    segmented->addSegment({
        .text = "CENTER",
        .segmentTex = texCommon.segmentedButton,
    });

    segmented->addSegment({
        .text = "R",
        .segmentTex = texCommon.segmentedButtonLast,
    });

    auto& ssegmented = M::Styles::segmented;

    ssegmented.segmentTex = texCommon.segmentedButton;
    ssegmented.segmentFirstTex = texCommon.segmentedButtonFirst;
    ssegmented.segmentLastTex = texCommon.segmentedButtonLast;


    scene->newSegmented({
        .rect = layout.positioned(segmentedSize, {0.5, 0.5}),
        .labels = {"Theme L", "Theme", "Theme R"},
    });

    // segmented->addSegment("ThemeL");
    // segmented->addSegment("Theme");
    // segmented->addSegment("ThemeR");

}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}
