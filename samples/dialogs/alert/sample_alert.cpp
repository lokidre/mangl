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

namespace sample {

class TestScreen: public Screen {
protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    sampleBuildBackground();
    sampleBuildStatusBar();

    auto scene = App::getScene();

    auto dialog = scene->newDialog({
        .type = DialogType::Confirm,
        .title = "Alert!",
        .message = "Sample message.",
        .confirmButtonText = "OK",
        .onConfirm = [this] {
            sampleSetStatusMessage("Confirmed.");
        },
    });

    sampleGridBuildScreenButton({
        .text = "Alert ...",
        .onPush = [this, dialog] {
            dialog->run();
            sampleSetStatusMessage("Showing Dialog...");
        },
    });

}

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

}  // namespace sample
