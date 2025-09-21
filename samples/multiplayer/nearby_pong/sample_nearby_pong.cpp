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

constexpr static auto serverMethod = "ws";
constexpr static auto serverAddress = "0.0.0.0";
constexpr static auto serverPort = "1234";



class TestScreen: public Screen {
    ButtonP advertiseButton_{};

    net::DiscoveryInfo di_{};
    std::unique_ptr<net::NearbyServer> wifiServer_{};
    std::unique_ptr<net::BluetoothServer> btServer_{};



    net::PeerContainer<net::Peer> peers_;

    bool pongPending_{};
    String pongData_;

    ButtonP startWiFiButton_{}, stopWiFiButton_{};
    ButtonP startBtButton_{}, stopBtButton_{};

    LabelP pingLabel_{};
    LabelP pongLabel_{};

    void startServer(net::NetworkBaseServer *server, StrView url);
    //void stopServer(net::NetworkBaseServer *server);

protected:
    void onStart() override;
    void onTimer(Timestamp timestamp) override;
};

void TestScreen::onStart() {
    sampleBuildBackground();
    sampleBuildTitle("Nearby Pong");

    startWiFiButton_ = sampleGridBuildScreenButton({
        .text = "Start Wi-Fi",
        .onPush = [this] {
            wifiServer_ = std::make_unique<net::NearbyServer>();
            auto serverUrl = apl::fmt("%s://%s:%s", serverMethod, serverAddress, serverPort);
            startServer(wifiServer_.get(), serverUrl);
            startWiFiButton_->disable();
            stopWiFiButton_->enable();
        },
    });

    stopWiFiButton_ = sampleGridBuildScreenButton({
        .text = "Stop Wi-Fi",
        .onPush = [this] {
            if (wifiServer_) {
                //wifiServer_.reset();
                wifiServer_->shutdown();
            }
            wifiServer_ = nullptr;
            sampleSetStatusMessage("Stopped Wi-Fi.");
            stopWiFiButton_->disable();
            startWiFiButton_->enable();
        }
    })->disable();

    sampleGridAdvanceRow();

    startBtButton_ = sampleGridBuildScreenButton({
        .text = "Start BT",
        .onPush = [this] {
            btServer_ = std::make_unique<net::BluetoothServer>();
            auto serverUrl = apl::fmt("%s://%s:%s", serverMethod, serverAddress, serverPort);
            startServer(btServer_.get(), serverUrl);

            startBtButton_->disable();
            stopBtButton_->enable();
        },
    });

    if (!net::BluetoothServer::isBluetoothAvailable()) {
        startBtButton_->setText("BT is not avail");
        startBtButton_->disable();
    }

    stopBtButton_ = sampleGridBuildScreenButton({
        .text = "Stop BT",
        .onPush = [this] {
            if (btServer_) {
                //wifiServer_.reset();
                btServer_->shutdown();
            }
            btServer_ = nullptr;
            sampleSetStatusMessage("Stopped Bluetooth.");
            stopBtButton_->disable();
            startBtButton_->enable();
        },
    })->disable();


    sampleGridAdvanceRow();


    pingLabel_ = sampleGridAddScreenLabel({});
    pongLabel_ = sampleGridAddScreenLabel({});

    sampleSetStatusMessage("Ready.");
}


void TestScreen::startServer(net::NetworkBaseServer* server, StrView url) {

    //wifiServer_ = std::make_unique<net::NearbyServer>();

    di_.alias = Env::deviceUserName();
    di_.uuid = ProtocolUuid;
    di_.proto = ProtocolName;
    di_.protv = ProtocolVersion;
    di_.appn = "sample_nearby_pong";
    di_.appv = "0.0.1";


    server->onError = [this](auto pk, auto err) {
        if (pk)
            sampleSetStatusMessage("Error: %d - %d, %s", pk, err.code, err.text);
        else
            sampleSetStatusMessage("Error: %d, %s", err.code, err.text);
    };

    server->onPeerFound = [this](auto pk, auto di) {
        sampleSetStatusMessage("Found: %d - %s", pk, di.build());

        peers_.create(pk, di);
    };


    server->onPeerState = [this](auto pk, auto st) {
        sampleSetStatusMessage("State: %d - %c", pk, M::getPeerStateString(st));

        if (auto peer = peers_.get(pk); peer) {
            peer->state = st;

            if (PeerState::Disconnected == st) {
                peers_.remove(peer);
            }
        }
    };

    server->onData = [this](auto pk, auto data) {
        //sampleSetStatusMessage("Recv: %d - %d", pk, data.size());

        pingLabel_->setText(data.strview());


        auto req = apl::strSplitTrim(data.strview(), ' ');

        if (!req.second.empty()) {
            pongData_ = apl::fmt("%s %s", "Pong", req.second);
        } else {
            pongData_.assign(data.strview());
        }
        pongPending_ = true;
    };

    server->start(url, di_);
    server->startAdvertising();

    sampleSetStatusMessage("Listening: %s...", url);
}

void TestScreen::onTimer(Timestamp timestamp) {
    if (pongPending_) {
        for (auto& peer: peers_) {
            if (wifiServer_) wifiServer_->send(peer->key, pongData_);
            if (btServer_) btServer_->send(peer->key, pongData_);
        }

        pongLabel_->setText(pongData_);

        pongPending_ = false;
    }
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
