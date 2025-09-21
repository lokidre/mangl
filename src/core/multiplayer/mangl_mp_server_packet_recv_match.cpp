/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_server.h"
#include "mangl_mp_game_instance_base.h"

namespace mangl::mmp {

void GameServerBase::processCommandMatch(MpPlayerP player, MpPacketCP packet) {
    switch (packet->ord) {
        using enum MessageOrder;

    case MatchCreate: {
        // TODO: verify if the player is allowed to create match
        auto matchKey = generateMatchKey();
        auto matchId = generateMatchId();

        auto match = matches_.create(matchKey, matchId);

        player->match = match;
        match->players.push_back(player);

        match->stage = MatchStage::Created;


        // Create game instance
        if (!match->gameInstance && onGameInstanceCreate) {
            match->gameInstance = onGameInstanceCreate();
            match->gameInstance->onServerInstance(this);
        }

        auto out = packetActionMatch(MsgOrd::MatchCreated);
        out->matchKey = match->matchKey;
        out->matchId = match->matchId;
        broadcastMatchStatus(match);

        if (match->gameInstance) {
            match->gameInstance->onServerActionPacket(out);
        }


        int a = 0;

        break;
    }

    case MatchFind:
        //debugMatchRecvStatus("Find", player);
        player->matching = true;
        break;

    case MatchJoin: 
    {
        //debugMatchRecvStatus("Join", player);
        //player->matching = true;

        // TODO: depending on the joining strategy
        auto matchByKey = matches_.get(packet->matchKey);
        auto matchById = matches_.byId(packet->matchId);
        auto match = player->match;

        if (match != matchByKey || match != matchById) {
            packetError(player, "Match Ids mismatch");
            break;
        }


        break;
    }

    case MatchReady:
        player->matchReadyReceived = true;
        break;

    default: 
        break;
    }

    if (player->match && player->match->gameInstance) player->match->gameInstance->onServerCommandPacket(player, packet);
}



}  // namespace mangl::mmp


