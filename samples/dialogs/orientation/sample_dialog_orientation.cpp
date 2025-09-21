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
    buildSampleBackground();

    auto scene = App::getScene();
    auto layout = Env::getSafeArea();

    Rect rect{layout.x + 10, layout.top() - 50, 120, 40};

    auto label = scene->newLabel({
        .text = "Press...",
        .pos = {layout.w / 2, rect.center().y},
        .flags = M::AlignLeft | M::AlignVCenter,
    });

    auto dialog = scene->newDialog({
        .type = DialogType::YesNo,
        .title = "Question",
        .message = "Are you sure?",
        .confirmButtonText = "Yes",
        .rejectButtonText = "No",
        .onConfirm = [label] {
            label->setText("Confirmed");
        },
        .rejectSignal = [label] {
            label->setText("Rejected");
        },
    });

    scene->newButton({
        .text = "Show",
        .rect = rect,
        .pushSignal = [dialog, label]() {
            label->setText("Showing...");
            dialog->setOrientation(M::Orientation0);
            dialog->run();
        },
    });

    rect.y -= rect.h;

    scene->newButton({
        .text = "Rotate",
        .rect = rect,
        .pushSignal = [dialog, label]() {
            label->setText("Rotating...");
            dialog->setOrientation(M::Orientation180);
            dialog->run();
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
