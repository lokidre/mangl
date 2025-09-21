/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_server_base.h"
#include "mangl_mp_client_base.h"


#include "../mangl_network.h"


namespace mangl::mmp {

void GameServerBase::setupNetwork() {

#if MANGL_MULTIPLAYER

    network_ = params_.serverNetwork;

    if (!network_) {
        switch (params_.connectionType) {
            using enum net::ConnectionType;
        case Nearby:
            network_ = new net::NearbyServer();
            params_.hostDiscovery = true;
            break;
        case Online:
            network_ = new net::OnlineServer();
            params_.hostDiscovery = false;
            break;
        default:
            MDebugTrap();
            break;
        }
    }


    MDebugTrapAssert(network_);

    if (!network_) return;

    using namespace std::placeholders;

    network_->onError = std::bind(&Self::processPeerError, this, _1, _2);
    network_->onPeerFound = std::bind(&Self::processPeerFound, this, _1, _2);
    network_->onPeerState = std::bind(&Self::processPeerState, this, _1, _2);
    network_->onData = std::bind(&Self::processDataRecv, this, _1, _2);
#endif
}

void GameServerBase::shutdownNetwork() {
#if MANGL_MULTIPLAYER

    if (network_) {
        network_->clearCallbacks();
        network_->shutdown();
        delete network_;
        network_ = nullptr;
    }
#endif
}

void GameServerBase::transmitData(MpPeerP peer, DataParamA data) {
    debugDumpPacket(true, peer, data);

#if MANGL_MULTIPLAYER

    if (peer->local) {
        auto client = dynamic_cast<GameClientBaseP>(peer->local);
        client->processDataRecv(client->hostPeer_.key, data);
        return;
    }

    network_->send(peer->key, data);
#endif
}

void GameServerBase::disconnect(MpPeerP peer) {
    network_->disconnect(peer->key);
    peer->setDisconnected();

    if (auto player = peer->player; player) {
        player->peerState = peer->state;
        player->peer = nullptr;
    }
}

void GameServerBase::disconnect(MpPlayerP player) {
    disconnect(player->peer);
    //player->peerState = PeerState::Disconnected;
    //player->peer = nullptr;
}


MpPeerP GameServerBase::localClientConnect(GameClientBaseP client, DiscoveryInfoA di) {
    // Create peer
    auto pk = net::NetworkBaseCommon::generatePeerKey();

    auto peer = peers_.create(pk, di);

    peer->state = PeerState::Found;
    peer->format = di.getFormat(net::DataTransferFormat::Packet);
    peer->local = client;
    peer->localConnectPending = true;
    peer->connectTimestamp = peer->lastDataTimestamp = currentTimestamp_;

    return peer;
}




MpPeerP GameServerBase::processPeerFound(PeerKey pk, DiscoveryInfoA di) {
    auto peer = peers_.create(pk, di);

    peer->state = PeerState::Found;
    peer->format = di.getFormat(net::DataTransferFormat::Packet);

    auto isNpc = di.getParam("npc");
    if (!isNpc.empty())
        peer->isNpc = true;

    debugConStatus("Found", pk, peer, di.build());

    //peer->player.alias = di.alias;

    return peer;
}

MpPeerP GameServerBase::processPeerState(PeerKey pk, PeerState ps) {
    auto peer = peers_.get(pk);

    debugConStatus("State", pk, peer, ps);

    if (!peer)
        return nullptr;

    peer->state = ps;

    if (PeerState::Connected == ps) {
        peer->connectTimestamp = peer->lastDataTimestamp = currentTimestamp_;
    }

    if (PeerState::Disconnected == ps) {
        // Check If this is a player/match
        auto player = peer->player;
        auto match  = player ? player->match : nullptr;

        processPlayerDisconnect(match, player);

        peers_.erase(peer);
        peer = nullptr;
    }

    return peer;
}

MpPeerP GameServerBase::processDataRecv(PeerKey pk, DataParamA data) {
    auto peer = peers_.get(pk);

    debugDataRecvStatus(pk, peer, data);

    if (!peer)
        return nullptr;

    debugDumpPacket(false, peer, data);


    auto packet = onPacketUnpack(peer, data);
    processPacket(peer, packet);

    return peer;
}


MpPeerP GameServerBase::processPeerError(PeerKey pk, ErrorInfo err) {
    auto peer = peers_.get(pk);

    debugConStatus("Error", pk, peer, apl::fmt("Code: %d, Message: %s", err.code, err.text));

    if (peer) {
    }

    return peer;
}



}  // namespace mangl::mmp


