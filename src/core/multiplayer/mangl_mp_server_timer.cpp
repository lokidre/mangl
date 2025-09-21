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

namespace mangl::mmp {

void GameServerBase::onBaseTimer(Timestamp timestamp) {
    // TODO: try/catch handling
    processTimerBody(timestamp);

    // In the end process all the queued packets
    // TODO: move to a separate thread probable
    while (!packetQueue_.empty()) {
        auto& item = packetQueue_.top();
        transmitData(item.peer, item.data);
        packetQueue_.pop();
    }
    //for (auto& item: queuedPackets_) {
    //    packetSend(item.peer, item.data);
    //}
    //queuedPackets_.clear();



    // Debugging
    if (params_.debugVerboseConState) {
        if (debugPrintStatusTimestamp_ == 0)
            debugPrintStatusTimestamp_ = timestamp + 1;
        if (timestamp >= debugPrintStatusTimestamp_) {
            debugPrintStats();
            debugPrintStatusTimestamp_ = 0;
        }
    }
}

void GameServerBase::processTimerBody(Timestamp timestamp) {
    for (auto peer: peers_) {
        // Check for the errored peers
        if (peer->isError) {
            disconnect(peer);
            continue;
        }

        if (peer->local) {
            auto client = static_cast<GameClientBaseP>(peer->local);

            if (peer->localConnectPending) {
                peer->localConnectPending = false;
                
                peer->state = PeerState::Found;
                client->processPeerFound(peer->key, params_.di);

                peer->state = PeerState::Connected;
                client->processPeerState(peer->key, PeerState::Connected);

                auto player = peer->player;

                int a= 0 ;
            }
        }
    }

    // Remove disconnected peers
    peers_.removeDisconnected();

    

    // Players cycle
    for (auto player: players_) {
        if (!player->helloReceived)
            continue;

        if (!player->welcomeSent) {
            player->welcomeSent = true;

            auto packet = packetActionSystem(MsgOrd::Welcome);
            packet->playerKey = player->key;
            packet->playerId = player->id;
            packetPush(player, packet);
            continue;
        }

        if (player->pingPending) {
            player->pingPending = false;

            auto packet = packetActionSystem(MsgOrd::Pong);
            packetPush(player, packet);
        }

    }


    //
    // Matchmaker timer
    //
    processMatchTimer(timestamp);

}


}  // namespace mangl::mmp

