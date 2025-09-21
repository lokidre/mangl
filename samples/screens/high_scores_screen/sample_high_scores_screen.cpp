/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>


class HighScoresScreen: public HighScoresScreenBase<Screen> {
public:
    HighScoresScreen() {
        //setup({
        //    .leaderboard = &Leaderboards::instance()->marathon,
        //});

        setup(&Leaderboards::instance()->marathon);
    }
};



class TestScreen: public Screen {
    using Self = TestScreen;

    Leaderboard* leaderboard_ = &Leaderboards::instance()->marathon;

protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    buildSampleBackground();

    gridAddScreenButton({
        .text = "High Scores",
        .onPush = App::pushScreenCreate<Self, HighScoresScreen>,
    });

    gridAddScreenButton({
        .text = "Post Score",
        .onPush = [this] {
            auto score = M::random.rangeInt(10, 100);
            leaderboard_->post(score);
            setStatusMessage("Posted: %d", score);
        },
    });

    gridAddScreenButton({
        .text = "Clear Scores",
        .onPush = [this] {
            leaderboard_->clear();
            setStatusMessage("Cleared Scores");
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

