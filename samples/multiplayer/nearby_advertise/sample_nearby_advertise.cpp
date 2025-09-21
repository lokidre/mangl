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

constexpr static auto serverMethod = "ws";
constexpr static auto serverAddress = "0.0.0.0";
constexpr static auto serverPort = "1234";


namespace sample::mp {

class TestScreen: public Screen {
    ButtonP advertiseButton_{};
    bool isAdvertising_{};
    String serverUrl_;

    net::DiscoveryInfo di_{};
    std::unique_ptr<net::NearbyServer> server_{};

protected:
    void onStart() override;
};

void TestScreen::onStart()
{
    buildSampleBackground();

    auto scene = App::getScene();
    auto safe = Env::getSafeArea();


    server_ = std::make_unique<net::NearbyServer>();

    di_.alias = Env::deviceUserName();
    di_.proto = ProtocolName;
    di_.protv = ProtocolVersion;
    di_.appn = "sample_nearby_discovery";
    di_.appv = "0.0.1";


    serverUrl_ = apl::fmt("%s://%s:%s", serverMethod, serverAddress, serverPort);

    server_->start(serverUrl_, di_);

    advertiseButton_ = scene->newButton({
        .text = "Start Advertising",
        .rect = safe.positioned({240, 60}, {0.5, 0.6}),
        .onPush = [this] {
            if (!isAdvertising_) {
                setStatusMessage("Advertising: %s", serverUrl_);

                advertiseButton_->setText("Stop Advertising");

                server_->startAdvertising();
            } else {
                setStatusMessage("Stopped advertising.");

                advertiseButton_->setText("Start Advertising");

                server_->stopAdvertising();
            }

            isAdvertising_ = !isAdvertising_;
        },
    });

}

} // namespace sample::mp


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

