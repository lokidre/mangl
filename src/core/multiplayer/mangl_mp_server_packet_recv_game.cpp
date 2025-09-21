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

void GameServerBase::processCommandGame(MpPlayerP player, MpPacketCP packet) {
    switch (packet->ord) {
        using enum MessageOrder;

    case GameMove: {

        if (player->match) {
            player->match->gameInstance->onServerCommandPacket(player, packet);
        }
        int a = 0;

        break;
    }

    case GameElement: break;

    case GameCharacter: {

        break;
    }

    case GameField: {
        break;
    }

    default: break;
    }

    if (player->match && player->match->gameInstance)
        player->match->gameInstance->onServerCommandPacket(player, packet);
}


} // namespace mangl::mmp
