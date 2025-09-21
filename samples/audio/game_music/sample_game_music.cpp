/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

namespace sample {
class TestScreen: public Screen {
    using Self = TestScreen;

    Timestamp ramp_{2};
    Timestamp delay_{1};
    Timestamp volume_{1};

    void genericMusicSetup();

protected:
    void onStart() override;
    void onRelease() override {
        GameMusic::clearCallbacks();
    }
};


void TestScreen::genericMusicSetup() {
    using enum MusicRes;
    GameMusic::setup(MFirst, MLast, {
        .welcomeTrack = Welcome,
        .welcomeTracks = {NightInKyoto, NoTimeToDie},
        .gameBeginTrack = NostalgicMoment,
        .gameEndTrack = UnderCover,
    });
}

void TestScreen::onStart() 
{
    sampleBuildBackground();
    sampleBuildStatusBar();

    setStatusMessage("Music %s", Music::settingEnabled() ? "enabled." : "disabled");

    // Loading sounds
    loadMusic();

    genericMusicSetup();
    //GameMusic::playWelcome();

    GameMusic::onTrackStart = [this](auto trackId) {
        sampleSetStatusMessage("Started: %d (%s)", trackId, Music::getFileName(trackId));
    };

    //GameMusic::onTrackStop = [this](auto trackId) {
    //    setStatusMessage("Stopped: %d (%s)", trackId, Music::getFileName(trackId));
    //};

    GameMusic::onTrackFinish = [this](auto trackId) {
        setStatusMessage("Finished: %d (%s)", trackId, Music::getFileName(trackId));
    };


    auto scene = App::getScene();

    sampleGridStart({});

    sampleGridBuildButton({
        .text = "Welcome",
        .onPush = GameMusic::playWelcome,
    });


    sampleGridBuildButton({
        .text = "Play Basic",
        .onPush = [] {
            GameMusic::setup<MusicRes>();
            GameMusic::play();
        },
    });




    gridAdvanceRow();

    sampleGridBuildScreenButton({
        .text = "Generic Setup",
        .onPush = [this] {  genericMusicSetup(); },
    });


    sampleGridBuildButton({
        .text = "Welcome",
        .onPush = [] {
            using enum MusicRes;
            GameMusic::setup(MFirst, MLast, {
                .welcomeTrack = Welcome,
            });
            GameMusic::playWelcome();
        },
    });


    gridAdvanceRow();


    scene->newButton({
        .text = "Game Begin",
        .rect = gridAddButton(),
        .onPush = GameMusic::playGameBegin,
    });

    scene->newButton({
        .text = "Game End",
        .rect = gridAddButton(),
        .onPush = GameMusic::playGameEnd,
    });




    scene->newButton({
        .text = "Next",
        .rect = gridAddButton(),
        .onPush = GameMusic::playNext,
    });

    scene->newButton({
        .text = "Stop",
        .rect = gridAddButton(),
        .onPush = GameMusic::stop,
    });




    gridStart({.direction = -1});

    scene->newSlider({
        .rect = gridAddSlider(),
        .textures = texCommon.slider,
        .position = Music::volume(),
        .onPosition = GameMusic::updateUserVolumePosition,
        .onPositionEnd = GameMusic::updateUserVolumeEnd,
    });

    scene->newCheckbox({
        .rect = gridAddCheckbox("Enable:"),
        .checked = Music::settingEnabled(),
        .onCheck = GameMusic::updateUserEnabledSetting,
    });

}




void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen<TestScreen>();
}

}  // namespace
