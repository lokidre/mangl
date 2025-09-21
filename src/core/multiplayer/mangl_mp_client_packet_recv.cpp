/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_client.h"

namespace mangl::mmp {

void GameClientBase::processPacket(MpPacketCP packet) {
    debugVerbosePacket(false, nullptr, packet);

    switch (packet->type) {
    case MessageType::Action: processPacketAction(packet); break;
    default: break;
    }

    if (packet->isGame()) {
        int a = 0;
    }
    onProcessPacket(&hostPeer_, packet);
}

void GameClientBase::processPacketAction(MpPacketCP packet) {
    switch (packet->cat) {
    case MessageCategory::System: processActionSystem(packet); break;
    case MessageCategory::Match: processActionMatch(packet); break;
    case MessageCategory::Game: 

        break;
    default:
        break;
    }
}

void GameClientBase::processActionSystem(MpPacketCP packet) {
    auto peer = &hostPeer_;

    switch (packet->ord) {
        using enum MessageOrder;
    case Welcome:
        debugSystemRecvStatus("Welcome", peer);
        status_.welcomeReceived = true;
        status_.playerKey = packet->playerKey;
        status_.playerId = packet->playerId;
        break;

    case Pong:
        status_.pongTimestamp = currentTimestamp_;
        break;

    default: break;
    }
}

void GameClientBase::processActionMatch(MpPacketCP packet) {
    debugMatchRecvStatus(packet);


    switch (packet->ord) {
        using enum MessageOrder;

    case MatchFound:
        status_.matchFound = true;
        status_.matchKey = packet->matchKey;
        status_.matchId = packet->matchId;
        break;

    case MatchJoined:
        status_.matchJoined = true;
        status_.matchFound = true;
        status_.matchKey = packet->matchKey;
        status_.matchId = packet->matchId;
        break;

    case MatchLaunch:
        status_.matchLaunched = true;
        break;

    default: break;
    }

    //if (onActionMatch) onActionMatch(&hostPeer_, packet);
}



}  // namespace 

