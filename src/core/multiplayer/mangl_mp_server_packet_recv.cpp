/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_server.h"

namespace mangl::mmp {

void GameServerBase::processPacket(MpPeerP peer, MpPacketCP packet) {

    // Check the packet correctness
    if (!packet->isValid()) {
        peer->setError(1, "Invalid Packet");
        return;
    }

    debugVerbosePacket(false, peer, packet);

    // Check the player status
    // First process the cases where the player is not in the system yet
    if (packet->isCommandSystem(MsgOrd::Hello)) {
        auto player = players_.byPeer(peer);

        if (player) {
            if (player->helloReceived)
                debugSystemRecvStatus("!Duplicate Hello", peer);
        }

        if (!player) {
        	auto pk = generatePlayerKey();
        	auto pid = generatePlayerId();

            player = players_.create(peer, pk, pid);
        }


        player->alias = player->name = peer->di.alias;


        player->helloReceived = true;
        //debugSystemRecvStatus("Hello", peer);
        return;
    }

    auto player = players_.byPeer(peer);
    if (!player) {
        MDebugTrap();
        return;
    }

    // Some chacking
    MDebugTrapAssert(player->peer == peer);

    if (packet->isCommand()) {
        if (packet->isSystem()) processCommandSystem(player, packet);
        if (packet->isMatch()) processCommandMatch(player, packet);
        if (packet->isCommandGame()) processCommandGame(player, packet);
    }
}

}  // namespace mangl::mmp


