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

MpPacketP GameClientBase::packetPrepare(MsgTyp type, MsgCat cat, MsgOrd ord) {
    auto packet = onPacketPrepare();

    packet->type = type;
    packet->cat = cat;
    packet->ord = ord;

    packet->messageId = currentPacketIndex_++;
    packet->messageTime = getCurrentEpochTime();

    return packet;
}


void GameClientBase::packetSend(MpPacketP packet) {
    auto peer = &hostPeer_;
    auto data = onPacketBuild(peer, packet);
    packetQueue_.push(peer, data);

    debugVerbosePacket(true, peer, packet);
}


}  // namespace 

