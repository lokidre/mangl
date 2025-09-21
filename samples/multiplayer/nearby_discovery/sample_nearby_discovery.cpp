/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <mangl_test.h>

#include "../shared/sample_mp_protocol.hpp"

#include <mangl/multiplayer.h>
#include <mangl/network.h>

namespace sample::mp {

class TestScreen: public Screen {
    ButtonP discoveryButton_{};
    LabelP detailsLabel_{};
    net::DiscoveryInfo di_{};
    std::unique_ptr<net::NearbyClient> nearbyClient_{};

protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    buildSampleBackground();


    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    detailsLabel_ = scene->newLabel({
        .pos = safe.position({0.5, 0.20}),
        .flags = M::AlignCenterAll,
        });


    nearbyClient_ = std::make_unique<net::NearbyClient>();

    nearbyClient_->onPeerFound = [this](auto pk, auto di) {
        auto url = di.getUrl();

        setStatusMessage("Found: %d - %s, %s", pk, di.alias, url);

        detailsLabel_->setText("Loc: %s", url);
    };

    nearbyClient_->onPeerState = [this](auto pk, auto st) {
        detailsLabel_->setText("State: %d - %c", pk, (char)st);
    };

    di_.alias = Env::deviceUserName();
    di_.proto = ProtocolName;
    di_.protv = ProtocolVersion;
    di_.appn = "sample_nearby_discovery";
    di_.appv = "0.0.1";


    discoveryButton_ = scene->newButton({
        .text = "Start Discovery",
        .rect = safe.positioned({200, 60}, {0.5, 0.6}),
        .onPush = [this] {
            if (!nearbyClient_->isDiscovering()) {
                setStatusMessage("Discovering...");
                discoveryButton_->setText("Stop Discovery");
                nearbyClient_->startDiscovery(di_);
            } else {
                setStatusMessage("Stopped discovering.");
                discoveryButton_->setText("Start Discovery");
                nearbyClient_->stopDiscovery();
            }
        },
    });

}

}  // namespace mangl::sample


namespace sample {

void App::onStart()
{
    if (sampleApplicationLayout_.h == 0)
        Env::setLayoutWidth(sampleApplicationLayout_.w);
    else
        Env::setLayout(sampleApplicationLayout_);

    switchScreen<mp::TestScreen>();
}

}  // namespace 
