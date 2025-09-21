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

namespace sample::mp {

class TestScreen: public Screen {

    using Self = TestScreen;

    LabelP dbgLabel_{};
    LabelP dbgConLabel_{};
    LabelP dbgSysLabel_{};
    LabelP dbgMatchLabel_{};
    LabelP dbgGameLabel_{};

    ButtonP joinButton_{};
    ButtonP releaseButton_{};

    SampleGameFrontend* gameClient_{};


protected:
    void onStart() override;

    void onRelease() override {
        if (gameClient_) gameClient_->clearCallbacks();
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



    joinButton_ = scene->newButton({
        .text = "Join Game",
        .rect = safe.positioned({200, 60}, {0.5, 0.6}),
        .onPush = [this] {

            gameClient_ = SampleGameFrontend::instance();

            gameClient_->onDebugMessage = [&](auto text) { dbgLabel_->setText(text); };
            gameClient_->onDebugMessageConnection = [&](auto text) { dbgConLabel_->setText(text); };
            gameClient_->onDebugMessageSystem = [&](auto text) { dbgSysLabel_->setText(text); };
            gameClient_->onDebugMessageMatch = [&](auto text) { dbgMatchLabel_->setText(text); };

            gameClient_->onMatch = [this](auto packet) {
                //auto packet = static_cast<SampleGamePacketCP>(pack);
                auto client = SampleGameFrontend::instance();

                if (packet->isMatchLaunch()) {
                    dbgGameLabel_->setText("Launching...");
                    client->sendMatchReady();
                }
                if (packet->isMatchCountdown()) {
                    dbgGameLabel_->setText("Countdown: %d", packet->value);
                }
            };


            gameClient_->setup({
                .connectionType = net::ConnectionType::Nearby,
            });

            gameClient_->start();

            joinButton_->setText("Started.");
            joinButton_->disable();  // Temporary

            releaseButton_->setText("Shutdown");
        },
    });

    releaseButton_ = scene->newButton({
        .text = "Not Launched",
        .rect = safe.positioned({200, 60}, {0.5, 0.6}).updY(currentY + 20),
        .onPush = [this] {
            if (gameClient_) gameClient_ = gameClient_->release();

            joinButton_->setText("Join Game")->enable();
        },
    });



    //nearbyClient_ = std::make_unique<net::NearbyClient>();
    //conClient_ = std::make_unique<net::NearbyClient>();

    //SampleGameClientBase::setup({
    //    .connectionType = net::ConnectionType::Nearby,
    //});
}

} // namespace 

namespace sample {

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new mp::TestScreen);
}

}  // namespace
