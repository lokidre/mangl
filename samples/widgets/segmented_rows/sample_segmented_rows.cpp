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

    buildSampleBackground();

    Size segmentedSize{280, 40};
    int numRows = 2;
    segmentedSize.h = segmentedSize.h * numRows;
    SegmentedP segmented{};

    auto& ssegmented = M::Styles::segmented;

    ssegmented.segmentTex = texCommon.segmentedButton;
    ssegmented.segmentFirstTex = texCommon.segmentedButtonFirst;
    ssegmented.segmentLastTex = texCommon.segmentedButtonLast;

    segmented = scene->newSegmented({
        .stateTag = "mangl-segmneted-sample-rows",
        .rect = layout.positioned(segmentedSize, {0.5, 0.5}),
    });

    for (int row = 0; row < numRows; ++row) {
        segmented->addRow();

        segmented->addSegment(apl::fmt("RowL: %d", row + 1));
        segmented->addSegment(apl::fmt("RowC: %d", row + 1));
        segmented->addSegment(apl::fmt("RowR: %d", row + 1));
    }

}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} // namespace
