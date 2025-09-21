/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <test_app.h>

#include <common_res.hpp>

#include <thread>
#include <chrono>

class TestScreen: public Screen {
    using Self = TestScreen;

    SfxId currentSfxId_{(int)SoundsRes::MBegin};

    SfxId nextSfxId() {
        auto sfxId = currentSfxId_++;
        if (currentSfxId_ >= (int)SoundsRes::MEnd)
            currentSfxId_ = (int)SoundsRes::MBegin;
        return sfxId;
    }

protected:
    void onStart() override;
//    void onTimer(Timestamp) override;
};


void TestScreen::onStart() 
{
    buildSampleBackground();
    buildStatusBar();

    setStatusMessage("Sounds %s", Sfx::settingEnabled() ? "enabled." : "disabled");

    // Loading sounds
    loadSfx();


    auto scene = App::getScene();

    auto safe = Env::getSafeArea();

    auto buttonSize = Size{140, 40};

    scene->newButton({
        .text = "Play",
        .rect = safe.positioned(buttonSize, {0.5, 0.9}),
        .onPush = [this] {
            Sfx::play(nextSfxId());
        },
    });

    scene->newButton({
        .text = "Play Delayed",
        .rect = safe.positioned(buttonSize, {0.5, 0.75}),
        .onPush = [this] {
            Sfx::playDelayed(nextSfxId(), 1);
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
