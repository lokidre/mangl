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

#include "mangl_mp_client_base.h"

namespace mangl::mmp {

MpPlayerP GameServerBase::createNpcPlayer()
{
    auto npc = onGameNpcCreate();
    if (!npc) return nullptr;

    npc->setup({
        .localServer = this,
    });

    npc->start();

    auto peer = localClientConnect(npc, npc->params_.di);
    if (!peer) {
        onGameNpcDestroy(npc);
        return nullptr;
    }

    npc->hostPeer_.key = peer->key;


    //auto peer = &npc->hostPeer_;

    peer->isNpc = true;
    npc->processPeerFound(peer->key, params_.di);

    peer->state = PeerState::Connected;

    npc->processPeerState(peer->key, peer->state);

    peer->localConnectPending = false;
    npc->status_.connected = true;


    npc->status_.helloSent = true;
    npc->status_.welcomeReceived = true;


    auto playerKey = generatePlayerKey();
    auto playerId = generatePlayerId();

    auto player = players_.create(peer, playerKey, playerId);

    player->helloReceived = true;
    player->alias = player->name = peer->di.alias;

    player->welcomeSent = true;

    npc->status_.playerKey = playerKey;

    player->matchReadyReceived = true;


    return player;
}



}  // namespace mangl::mmp


