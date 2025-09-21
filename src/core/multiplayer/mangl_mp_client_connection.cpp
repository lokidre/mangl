/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_client.h"
#include "mangl_mp_server_base.h"

#include "../mangl_network.h"


namespace mangl::mmp {

void GameClientBase::setupNetwork() {

#if MANGL_MULTIPLAYER
    clientPeer_.di = params_.di;

    clientPeer_.format = params_.dataTransferFormat;
    clientPeer_.di.setFormat(clientPeer_.format);


    //network_ = params_.network;

    if (!network_) {
        switch (params_.connectionType) {
            using enum net::ConnectionType;
        case Local:
            params_.hostDiscovery = false;
            MDebugAssert(params_.localServer);
            break;

        case Nearby:
            network_ = new net::NearbyClient();
            params_.hostDiscovery = true;
            break;
        case Online:
            network_ = new net::OnlineClient();
            params_.hostDiscovery = false;
            break;
        default:
            MDebugTrap();
            break;
        }
    }


    if (network_) {
        using namespace std::placeholders;

        network_->onError = std::bind(&Self::processPeerError, this, _1, _2);
        network_->onPeerFound = std::bind(&Self::processPeerFound, this, _1, _2);
        network_->onPeerState = std::bind(&Self::processPeerState, this, _1, _2);
        network_->onData = std::bind(&Self::processDataRecv, this, _1, _2);
    }


    if (params_.dataTransferFormat != net::DataTransferFormat::Unknown) {
        clientPeer_.di.setFormat(params_.dataTransferFormat);
    }
#endif
}

void GameClientBase::networkConnect() {
#if MANGL_MULTIPLAYER
    if (network_) {
        network_->connect(hostUrl_, clientPeer_.di);
        return;
    }
#endif
}


void GameClientBase::shutdownNetwork() {
#if MANGL_MULTIPLAYER
    if (network_) {
        network_->clearCallbacks();
        delete network_;
        network_ = nullptr;
    }
#endif
}


void GameClientBase::transmitData(DataParamA data) {
#if MANGL_MULTIPLAYER
    debugDumpPacket(true, nullptr, data);

    if (network_) {
        network_->send(data);
        return;
    }

    if (params_.localServer) {
        params_.localServer->processDataRecv(hostPeer_.key, data);
    }
#endif
}





MpPeerP GameClientBase::processPeerError(PeerKey pk, ErrorInfo err) {
    debugConStatus("Error", pk, &hostPeer_, apl::fmt("Code: %d, Message: %s", err.code, err.text));
    return nullptr;
}

MpPeerP GameClientBase::processPeerFound(PeerKey pk, net::DiscoveryInfoA di) {
    hostUrl_ = di.getUrl();

    status_.pendingConnect = true;

    auto peer = &hostPeer_;

    peer->key = pk;
    peer->di = di;

    // Check for the supported data formats
    // TODO: implement more sophisticated stuff
    peer->format = clientPeer_.format;


    debugConStatus("Found", pk, peer, di.build());

    //hostPeer_.format = hostPeer_.di.getFormat(net::DataTransferFormat::Packet); // (params_.dataTransferFormat
    //hostPeer_.di.setFormat(dataFormat);

    //hostPeer_.player.alias = di.alias;

    return peer;
}

MpPeerP GameClientBase::processPeerState(PeerKey pk, PeerState ps) {
    debugConStatus("State", pk, &hostPeer_, ps);


    // TODO: disconnections, etc

    status_.connected = PeerState::Connected == ps;

    if (status_.connected) {
        //pingTrigger_ = 0.1;
    }

    return nullptr;
}

void GameClientBase::processDataRecv(PeerKey pk, DataParamA data) {
    auto peer = &hostPeer_;

    debugDataRecvStatus(pk, peer, data);

    auto packet = onPacketUnpack(peer, data);
    processPacket(packet);

    onPacketProcess(packet);
}


}  // namespace mangl::mmp
