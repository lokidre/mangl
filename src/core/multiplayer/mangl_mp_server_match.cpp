/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_server_base.h"
#include "mangl_mp_game_instance_base.h"

namespace mangl::mmp {

static void updateMatchPacket(GamePacketBaseP packet, MpMatchP match) {
    packet->matchKey = match->matchKey;
    packet->matchId = match->matchId;
    packet->matchStage = match->stage;
    packet->players = match->packetPlayers;
}

void GameServerBase::packetBroadcast(MpMatchP match, MpPacketP packet) {
    for (auto player: match->players) {
        packetPush(player, packet);
    }
}


void GameServerBase::broadcastMatchPacket(MpMatchP match, MpPacketP packet) {
    if (match->gameInstance) match->gameInstance->onServerActionPacket(packet);

    for (auto player: match->players) {
        packetPush(player, packet);
    }
}



void GameServerBase::broadcastMatchStatus(MpMatchP match) {
    match->updatePacketPlayers();

    auto packet = packetActionMatch(MsgOrd::MatchStatus);
    updateMatchPacket(packet, match);

    broadcastMatchPacket(match, packet);
}


void GameServerBase::launchMatch(MpMatchP match) {
    match->stage = MatchStage::Launched;

    match->updatePacketPlayers();


    auto packet = packetActionMatch(MsgOrd::MatchLaunch);
    updateMatchPacket(packet, match);

    // Send match status first
    if (match->gameInstance)
        match->gameInstance->processMatchLaunch(match, packet);


    for (auto player: match->players) {
        if (!player->isNpc())
            player->matchReadyReceived = false;
    }

    broadcastMatchPacket(match, packet);
}

void GameServerBase::beginMatch(MpMatchP match) {
    match->stage = MatchStage::Playing;

    auto packet = packetActionMatch(MsgOrd::MatchBegin);
    broadcastMatchPacket(match, packet);

    if (match->gameInstance) 
        match->gameInstance->processMatchBegin(currentTimestamp_);

}




void GameServerBase::discardMatch(MpMatchP match) {

    // Kill the game instance
    if (match->gameInstance) {
        if (match->gameInstance && onGameInstanceDestroy) {
            onGameInstanceDestroy(match->gameInstance);
            match->gameInstance = nullptr;
        }
    }
        
    
    // Remove or disconnect the players
    for (auto p: match->players) {
        p->match = nullptr;

        if (PeerState::Disconnected == p->peerState) {
            players_.remove(p);
        }
    }

    matches_.erase(match);
}


void GameServerBase::processPlayerDisconnect(MpMatchP match, MpPlayerP player) {
    if (!player) return;

    // If the player is part of the match and the match started
    if (match) {
        // Check the match status

        player->peerState = PeerState::Disconnected;
        player->peer = nullptr;

        int connectedPlayers = 0;
        for (auto& mp: match->players) {
            // Disconnect all NPCs
            if (mp->peer && mp->peer->isNpc) {
                disconnect(mp);
                continue;
            }

            if (mp->peerState != PeerState::Disconnected)
                ++connectedPlayers;
        }

        // If every players is disconnected, then discard the match
        if (!connectedPlayers) {
            discardMatch(match);
        }

    } else {
        players_.remove(player);
    }
}




}  // namespace mangl::mmp

