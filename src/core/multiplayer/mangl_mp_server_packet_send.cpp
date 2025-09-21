/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_server_base.h"

#include "../mangl_network.h"

namespace mangl::mmp {

MpPacketP GameServerBase::packetPrepare(MsgTyp type, MsgCat cat, MsgOrd ord) {
    auto packet = onPacketPrepare();

    packet->type = type;
    packet->cat = cat;
    packet->ord = ord;

    //packet->mmpTransmissionPending = true;

    packet->messageId = currentPacketIndex_++;
    packet->messageTime = getCurrentEpochTime();


    return packet;
}

//MpPacketP GameServerBase::packetPrepare(MpPlayerP player, MsgTyp type, MsgCat cat, MsgOrd ord) {
//    auto packet = packetPrepare(player->peer, type, cat, ord);
//
//    //packet->playerKey = player->key;
//    //packet->playerId = player->id;
//
//    return packet;
//}



//void GameServerBase::packetSend(PlayerP player, BPacketP packet) {
//    packetSend(player->peer, packet);
//}

void GameServerBase::packetPush(MpPeerP peer, MpPacketP packet) {
    auto data = onPacketBuild(peer, packet);
    packetQueue_.push(peer, data);


    debugVerbosePacket(true, peer, packet);


    //auto& item = queuedPackets_.emplace_back();
    //item.peer = peer;
    //item.data.resize(data.size());
    //std::memcpy(item.data.data(), data.data(), data.size());

    //packet->mmpTransmissionPending = false;
}

//void GameServerBase::packetSend(MpPeerP peer, DataParamA data) {

    //sendPacketData(peer, data);
    //onPacketSend(peer, data);
//}

}  // namespace mangl::mmp


