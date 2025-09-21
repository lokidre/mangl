/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_app.h"
#include "../../../mangl/src/core/mangl_schedule_trigger.h"
#include <mangl/screen.h>

#include <mangl/timer.h>

class TestScreen: public ScreenBase {
    using Self = TestScreen;

public:
    static ScheduleTriggerTimed tik;

protected:
    void onStart() override;
    void onTimer(Timestamp timestamp) override;
};

void TestScreen::onTimer(Timestamp timestamp) {
    auto scene = App::getScene();
    auto pauseGrid = scene->newGridLayout({});
    // tik.trigger(1000);
//    pauseGrid->addItem({
//        .pos = {0, 0},
//        .size = {100, 100},
//    });
    if (tik.triggered()) {
        scene->newBox(M::Red, {100, 100, 100, 100});
        scene->newDialog({
            .type = DialogType::YesNo,
            .title = "PAUSED",
            .grid = pauseGrid,
            .confirmButtonText = "RESUME",
            .rejectButtonText = "QUIT",
        });
    }
}
ScheduleTriggerTimed TestScreen::tik{};
void TestScreen::onStart() {
    auto scene = App::getScene();
    auto layout = Env::fullLayout();

    scene->newButton({
        .rect = {layout.right() - 45 * 1.2, layout.top() - 45, 45, 45},
        .pushSignal = [] { tik.trigger(5); },
    });


}

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new TestScreen);
}

