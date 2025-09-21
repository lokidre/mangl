/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include "../shared/sample_game_frontend.hpp"
#include "../shared/sample_game_screen.hpp"

namespace sample::mp {

class TestScreen: public Screen {

    using Self = TestScreen;

    LabelP dbgLabel_{};
    LabelP dbgConLabel_{};
    LabelP dbgSysLabel_{};
    LabelP dbgMatchLabel_{};
    LabelP dbgGameLabel_{};

    ButtonP playButton_{};

    SampleGameMode* gameMode_ = SampleGameMode::instance();


    //SampleGameFrontend* gameClient_{};

    //void onActionSystem(SampleGamePacketCP packet);
    //void onActionMatch(SampleGamePacketCP packet);

protected:
    void onStart() override;

    void onRelease() override {
        auto& client = SampleGameMode::instance()->client;
        if (client)
            client->clearCallbacks();
    }
};


void TestScreen::onStart()
{
    buildSampleBackground();
    auto statusRect = buildStatusBar();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();


    auto currentY = statusRect.top();

    auto makeDebugLabel = [&](StrView title) {
        const auto& m = testStatusTextMargins_;

        auto label = scene->newLabel({
            .pos = {safe.x + m.w, currentY + m.h},
            .textSize = testStatusTextSize_,
            .flags = M::AlignLeft | M::AlignBottom,
            })->setText("%s:", title);

        label = scene->newLabel({
            .pos = {safe.x + m.w + testStatusTextSize_, currentY + m.h},
            .textSize = testStatusTextSize_,
            .flags = M::AlignLeft | M::AlignBottom,
            });

        currentY += testStatusTextSize_ + m.h * 2;

        return label;
    };

    dbgLabel_ = makeDebugLabel("D");
    dbgConLabel_ = makeDebugLabel("C");
    dbgSysLabel_ = makeDebugLabel("S");
    dbgMatchLabel_ = makeDebugLabel("M");
    dbgGameLabel_ = makeDebugLabel("G");

    playButton_ = scene->newButton({
        .text = "PLAY",
        .rect = safe.positioned({200, 60}, {0.5, 0.6}),
        .onPush = [this] {
            gameMode_->client = SampleGameFrontend::instance();

            //App::pushScreenCreate<Self, SampleGameScreen>();
            //return;

            auto client = gameMode_->client;
            client->onDebugMessage = [&](auto text) { dbgLabel_->setText(text); };
            client->onDebugMessageConnection = [&](auto text) { dbgConLabel_->setText(text); };
            client->onDebugMessageSystem = [&](auto text) { dbgSysLabel_->setText(text); };
            client->onDebugMessageMatch = [&](auto text) { dbgMatchLabel_->setText(text); };

            client->onMatch = [this](auto packet) {
                if (packet->isMatchLaunch()) {
                    App::pushScreenCreate<Self, SampleGameScreen>();
                    return;
                }
            };

            client->setup({
                .connectionType = net::ConnectionType::Nearby,
            });

            client->start();

            playButton_->setText("Connecting...")->disable();

        },
    });

}

}  // namespace sample::mp

namespace sample {

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen<mp::TestScreen>();
}

}  // namespace sample
