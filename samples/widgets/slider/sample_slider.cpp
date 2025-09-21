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
    buildStatusBar();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    auto center = safe.center();

    Size sliderSize{safe.w - 50, 40};
    auto sliderRect = safe.positioned(sliderSize, {0.5, 0.75});

    scene->newSlider({
        .rect = sliderRect,
        .textures = texCommon.slider,
        .position = 0.2,
        .onPositionBegin = [this](Real position) {
            setStatusMessage("Begin: %.2f", position);
        },
        .onPosition = [this](Real position) {
            setStatusMessage("Position: %.2f", position);
        },
        .onPositionEnd = [this](Real position) {
            setStatusMessage("End: %.2f", position);
        },
    });


    sliderRect = safe.positioned(sliderSize, {0.5, 0.5});

    scene->newSlider({
        .stateTag = "slider_with_id",
        .id = 3,
        .rect = sliderRect,
        .textures = texCommon.slider,
        .position = 0.3,
        .onPositionBeginId = [this](auto id, Real position) {
            setStatusMessage("Begin (%d): %.2f", id, position);
        },
        .onPositionId = [this](auto id, Real position) {
            setStatusMessage("Position (%d): %.2f", id, position);
        },
        .onPositionEndId = [this](auto id, Real position) {
            setStatusMessage("End (%d): %.2f", id, position);
        },
    });


    //
    // Slider without textures
    //
    // TODO:
    //sliderRect = safe.positioned(sliderSize, {0.5, 0.35});

    //scene->newSlider({
    //    .rect = sliderRect,
    //});

}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}  // namespace

