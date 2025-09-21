/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include "../shared/sample_game_backend.hpp"

namespace sample::mp {

constexpr static auto serverMethod = "ws";
constexpr static auto serverAddress = "0.0.0.0";
constexpr static auto serverPort = "13781";


class TestScreen: public Screen {

    LabelP dbgLabel_{};
    LabelP dbgConLabel_{};
    LabelP dbgSysLabel_{};
    LabelP dbgMatchLabel_{};
    LabelP dbgGameLabel_{};

    ButtonP createButton_{};
    ButtonP releaseButton_{};

    SampleGameServer* gameServer_{};

protected:
    void onStart() override;

    void onRelease() override {
        if (gameServer_) gameServer_->clearCallbacks();
    }

};

void TestScreen::onStart() {
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


    createButton_ = scene->newButton({
        .text = "Create Game",
        .rect = safe.positioned({200, 60}, {0.5, 0.6}),
        .onPush = [this] {

            gameServer_ = SampleGameServer::instance();
            gameServer_->onDebugMessage = [&](auto text) { dbgLabel_->setText(text); };
            gameServer_->onDebugMessageConnection = [&](auto text) { dbgConLabel_->setText(text); };
            gameServer_->onDebugMessageSystem = [&](auto text) { dbgSysLabel_->setText(text); };
            gameServer_->onDebugMessageMatch = [&](auto text) { dbgMatchLabel_->setText(text); };


            gameServer_->setup({
                .connectionType = net::ConnectionType::Nearby,
                .hostUrl = apl::fmt("%s://%s:%d", serverMethod, serverAddress, serverPort),
                .debugVerboseAll = true,
                .debugVerboseConData = true,
            });

            gameServer_->start();

            createButton_->setText("Started.")->disable(); // Temporary

            releaseButton_->setText("Shutdown");
        },
    });

    releaseButton_ = scene->newButton({
        .text = "Not Launched",
        .rect = safe.positioned({200, 60}, {0.5, 0.6}).updY(currentY + 20),
        .onPush = [this] {
            if (gameServer_) gameServer_ = gameServer_->release();

            createButton_->setText("Create Game")->enable();
        },
    });

}

} // namesace sample::mp

namespace sample {

void App::onStart() {
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen(new mp::TestScreen);
}

} // namespace sample
