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
    sampleBuildBackground();

    sampleGridBuildScreenButton({
        .text = "Button",
        .onPush = [this] {
            sampleSetStatusMessage("Button pressed.");
        },
    });

    sampleGridBuildScreenButton({
        .text = "Disabled",
        .onPush = [this] {
            sampleSetStatusMessage("Disabled pressed.");
        },
    })->disable();

}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

} // namespace
