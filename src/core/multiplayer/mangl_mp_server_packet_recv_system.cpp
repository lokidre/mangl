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

void GameServerBase::processCommandSystem(MpPlayerP player, MpPacketCP packet) {
    switch (packet->ord) {
    case MessageOrder::Ping: 
        player->pingPending = true;
        player->pingTimestamp = currentTimestamp_;
        player->pingUserTime = packet->messageTime;

        debugDataRecvStatus(player, apl::fmt("Ping: %d", packet->messageId));
        break;

    default: 
        break;
    }
}

}  // namespace mangl::mmp

