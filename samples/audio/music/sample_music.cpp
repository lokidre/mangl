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

    MusicTrackId currentMusicTrack_{(MusicTrackId)MusicRes::MBegin};

    MusicTrackId nextMusicTrack() {
        auto trackId = currentMusicTrack_++;
        if (currentMusicTrack_ >= (MusicTrackId)MusicRes::MEnd)
            currentMusicTrack_ = (MusicTrackId)MusicRes::MBegin;
        return trackId;
    }

    Timestamp ramp_{6};
    Timestamp delay_{5};
    Timestamp volume_{1};
    Timestamp crossfadeGap_{3};

protected:
    void onStart() override;
    void onRelease() override {
        Music::clearCallbacks();
    }
};


void TestScreen::onStart() 
{
    buildSampleBackground();
    buildStatusBar();

    setStatusMessage("Music %s", Music::settingEnabled() ? "enabled." : "disabled");

    // Loading sounds
    loadMusic();

    Music::onStart = [this](auto trackId) {
        setStatusMessage("Started: %d (%s)", trackId, apl::extractFileName(Music::getFileName(trackId)));
    };


    Music::onFinish = [this](auto trackId) {
        setStatusMessage("Finished: %d (%s)", trackId, apl::extractFileName(Music::getFileName(trackId)));

        // Play next one
        Music::play(nextMusicTrack(), ramp_);
    };



    auto scene = App::getScene();

    auto safe = Env::getSafeArea();

    auto buttonSize = Size{120, 40};

    scene->newButton({
        .text = "Play",
        .rect = gridAddButton(),
        .onPush = [this] {
            Music::play(nextMusicTrack());
        },
    });

    scene->newButton({
        .text = "Stop",
        .rect = gridAddButton(),
        .onPush = [this] {
            Music::stop();
        },
    });


    scene->newButton({
        .text = "Play Ramp",
        .rect = gridAddButton(),
        .onPush = [this] {
            Music::setVolume(volume_);
            Music::play(nextMusicTrack(), ramp_);
        },
    });

    scene->newButton({
        .text = "Stop Fade",
        .rect = gridAddButton(),
        .onPush = [this] {
            Music::stop(ramp_);
        },
    });


    scene->newButton({
        .text = "Play Delayed",
        .rect = gridAddButton(),
        .onPush = [this] {
            setStatusMessage("Playing in %g, ramp %g...", delay_, ramp_);
            Music::playDelayed(nextMusicTrack(), delay_, ramp_);
        },
    });

    scene->newButton({
        .text = "Stop Delay",
        .rect = gridAddButton(),
        .onPush = [this] {
            setStatusMessage("Stopping in %g, ramp %g...", delay_, ramp_);
            Music::stopDelayed(delay_, ramp_);
        },
    });


    scene->newButton({
        .text = "Crossfade",
        .rect = gridAddButton(),
        .onPush = [this] {
            Music::playCrossfade(nextMusicTrack(), ramp_, crossfadeGap_);
        },
    });


    scene->newButton({
        .text = "C/F Delayed",
        .rect = gridAddButton(),
        .onPush = [this] {
            Music::setVolume(volume_);
            Music::playCrossfadeDelayed(nextMusicTrack(), delay_, ramp_, crossfadeGap_);
        },
    });


    scene->newButton({
        .text = "Pause",
        .rect = gridAddButton(),
        .onPush = [this] {
            setStatusMessage("Paused.");
            Music::pause();
        },
    });

    scene->newButton({
        .text = "Resume",
        .rect = gridAddButton(),
        .onPush = [this] {
            setStatusMessage("Resume");
            Music::resume();
        },
    });




    gridStart({.direction = -1});

    scene->newSlider({
        .rect = gridAddSlider(),
        .textures = texCommon.slider,
        .position = Music::volume(),
        .onPosition = [](Real position) {
            Music::updateUserVolumePosition(position, MusicRes::Welcome);
        },
        .onPositionEnd = [](Real position) {
            Music::updateUserVolumeEnd(position, MusicRes::Welcome);
        },
    });

    scene->newCheckbox({
        .rect = gridAddCheckbox("Enable:"),
        .checked = Music::settingEnabled(),
        .onCheck = [this](bool checked) {
            Music::updateUserEnabledSetting(checked, MusicRes::Welcome);
            setStatusMessage("Music %s", checked ? "enabled." : "disabled");
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
