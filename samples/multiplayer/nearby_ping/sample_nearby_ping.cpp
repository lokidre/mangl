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
    net::DiscoveryInfo di_{};
    String hostUrl_;


    std::unique_ptr<net::NearbyClient> wifiClient_{};
    std::unique_ptr<net::BluetoothClient> bluetoothClient_{};

    ButtonP startWiFiButton_{}, stopWiFiButton_{};
    ButtonP startBtButton_{}, stopBtButton_{};

    LabelP pingLabel_{};
    LabelP pongLabel_{};

    struct ClientContext {
        bool connected{};
        bool pendingConnect{};
    };

    ClientContext wifiContext_{}, bleContext_{};

    ScheduleTriggerTimed pingTrigger_{};
    int pingCounter_{1};

    void startClient(net::NetworkBaseClient* client, ClientContext* ctx);

protected:
    void onStart() override;
    void onTimer(Timestamp timestamp) override;
};

void TestScreen::onStart()
{
    buildSampleBackground();
    sampleBuildTitle("Nearby Ping");


    startWiFiButton_ = sampleGridBuildScreenButton({
        .text = "Connect Wi-Fi",
        .onPush = [this] {
            wifiClient_ = std::make_unique<net::NearbyClient>();
            startClient(wifiClient_.get(), &wifiContext_);
            startWiFiButton_->disable();
            stopWiFiButton_->enable();
        },
    });

    stopWiFiButton_ = sampleGridBuildScreenButton({
        .text = "Disconnect Wi-Fi",
        .onPush = [this] {
            wifiContext_ = {};
            if (wifiClient_) {
                //wifiServer_.reset();
                wifiClient_->shutdown();
            }
            wifiClient_ = nullptr;
            sampleSetStatusMessage("Stopped Wi-Fi.");
            stopWiFiButton_->disable();
            startWiFiButton_->enable();
        }
    })->disable();


    sampleGridAdvanceRow();

    startBtButton_ = sampleGridBuildScreenButton({
        .text = "Connect BT",
        .onPush = [this] {
            bluetoothClient_ = std::make_unique<net::BluetoothClient>();
            startClient(bluetoothClient_.get(), &bleContext_);
            startBtButton_->disable();
            stopBtButton_->enable();
        },
    });

    if (!net::BluetoothClient::isBluetoothAvailable()) {
        startBtButton_->setText("BT is not avail");
        startBtButton_->disable();
    }

    stopBtButton_ = sampleGridBuildScreenButton({
        .text = "Disconnect BT",
        .onPush = [this] {
            bleContext_ = {};
        },
    })->disable();



    sampleGridAdvanceRow();

    pingLabel_ = sampleGridAddScreenLabel({});
    pongLabel_ = sampleGridAddScreenLabel({});

    sampleSetStatusMessage("Ready.");
}

void TestScreen::onTimer(Timestamp timestamp) {
    if (wifiContext_.pendingConnect) {
        wifiClient_->connect(hostUrl_, di_);
        wifiContext_.pendingConnect = false;
    }

    if (bleContext_.pendingConnect) {
        bluetoothClient_->connect(hostUrl_, di_);
        bleContext_.pendingConnect = false;
    }


    if (pingTrigger_) {
        auto data = apl::fmt("Ping %d", pingCounter_++);

        pingLabel_->setText(data);

        if (wifiClient_ && wifiContext_.connected) wifiClient_->send(data);
        if (bluetoothClient_ && bleContext_.connected) bluetoothClient_->send(data);

        pingTrigger_ = 1;
    }

}

void TestScreen::startClient(net::NetworkBaseClient* client, ClientContext* ctx) {
    client->onError = [this](auto pk, auto err) {
        sampleSetStatusMessage("Error: %d - %d, %s", pk, err.code, err.text);
    };


    client->onPeerFound = [this, ctx](auto pk, auto di) {
        hostUrl_ = di.getUrl();

        sampleSetStatusMessage("Found: %d - %s, %s", pk, di.alias, hostUrl_);

        pongLabel_->setText("Url: %s", hostUrl_);

        ctx->pendingConnect = true;
    };

    client->onPeerState = [this, ctx](auto pk, auto ps) {
        sampleSetStatusMessage("State: %d - %s", pk, M::getPeerStateString(ps));

        pongLabel_->setText("State: %d - %s", pk, M::getPeerStateString(ps));

        ctx->connected = PeerState::Connected == ps;

        if (ctx->connected) {
            pingTrigger_ = 1;
        }
    };

    client->onData = [this, ctx](auto pk, auto data) {
        pongLabel_->setText(data.strview());
    };



    di_.alias = Env::deviceUserName();
    di_.uuid = ProtocolUuid;
    di_.proto = ProtocolName;
    di_.protv = ProtocolVersion;
    di_.appn = "sample_nearby_ping";
    di_.appv = "0.0.1";

    sampleSetStatusMessage("Discovering...");
    client->startDiscovery(di_);
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
