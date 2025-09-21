/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_base.h"

namespace mangl::mmp {


String MpGameBase::debugMakePeerInfo(PeerKey pk, MpPeerCP peer) const noexcept {
    auto key = peer ? peer->key : pk;

    if (!key)
        return {};

    if (peer) {
        //return apl::fmt("%s(%d)", peer->di.alias, key);
        return apl::fmt("%d@%s", key, peer->di.alias);
    } 

    return std::to_string(key);
}

void MpGameBase::debugConStatus(StrView prefix, PeerKey pk, MpPeerCP peer, PeerState ps) {
    if (!params_.debugVerboseConState) return;

    String ss = "Unknown";

    using enum PeerState;
    switch (ps) {
    case Found: ss = "Found"; break;
    case Accepted: ss = "Accepted"; break;
    case Lost: ss = "Lost"; break;
    case Error: ss = "Error"; break;
    case Connecting: ss = "Connecting"; break;
    case Connected: ss = "Connected"; break;
    case Disconnected: ss = "Disconnected"; break;
    default: break;
    }

    //debugConStatus(prefix, pk, peer, apl::fmt("%c", (char)ps));
    debugConStatus(prefix, pk, peer, ss);
}

void MpGameBase::debugConStatus(StrView prefix, PeerKey pk, MpPeerCP peer, StrView suffix) {
    if (!params_.debugVerboseConState) return;

    String str;

    if (!pk && !peer) {
        str = apl::fmt("~ %s: %s", prefix, suffix);
    } else {
        str = apl::fmt("~ %s: %s - %s", prefix, debugMakePeerInfo(pk, peer), suffix);
    }

    if (onDebugMessage) onDebugMessage(str);
    if (onDebugMessageConnection) onDebugMessageConnection(str);
}



String MpGameBase::debugMakeMatchInfo(MpPacketCP packet) const noexcept {
    String str;
    str += apl::fmt("stage: %d", (int)packet->matchStage);
    if (packet->matchKey) str += apl::fmt(", key: %d", packet->matchKey);
    if (!packet->matchId.empty()) str += apl::fmt(", id: %d", packet->matchId);

    if (!packet->players.empty())
        str += apl::fmt(", players: %d", packet->players.size());

    return str;
}


void MpGameBase::debugDumpPacket(bool send, MpPeerCP peer, DataParamA data) {

    if (!params_.debugPacketLogEnabled) return;

    auto timestamp = apl::formatTimestampEx(apl::getLocalTime()).substr(6);
    auto ext = net::DiscoveryInfo::makeFormat(peer->format);
    auto type = send ? "s" : "r";
    auto peerInfo = peer ? apl::fmt("-%d-%s", peer->key, peer->di.alias) : "";
    auto filename = apl::fmt("packet-%s-%s%s", timestamp, type, peerInfo);
    auto filepath = apl::makeFilePathExt(params_.debugPacketLogDir, filename, ext);

   
    apl::saveFile(filepath, data);
}


void MpGameBase::debugVerbosePacket(bool send, MpPeerCP peer, MpPacketCP packet) {

    if (!params_.debugVerbosePacket) return;

    auto dir = send ? '>' : '<';
    auto text = apl::fmt("%c Packet:%c%c%c", dir, (char)packet->type, (char)packet->cat, (char)packet->ord);
    text += " " + debugMakePeerInfo(peer);

    if (onDebugMessage) onDebugMessage(text);
    if (onDebugMessagePacket) onDebugMessagePacket(text);


    //switch (packet->type) {
    //case MsgTyp::Action: 
    //    switch (packet->cat) {
    //    case MsgCat::System:
    //        switch (packet->ord) {
    //            using enum MessageOrder;
    //            case Welcome: return debugSystemStatus("Welcome", peer, send);
    //        }
    //        break;
    //    case MsgCat::Match:
    //        break;
    //    }
    //    break;

    //case MsgTyp::Command:
    //    switch (packet->cat) {
    //    case MsgCat::System:
    //        switch (packet->ord) {
    //            using enum MessageOrder;
    //        case Hello: return debugSystemStatus("Hello", peer, send);
    //        case Pong: return debugDataSendStatus(peer->player, apl::fmt("Pong: %d", packet->messageId));
    //        default: break;
    //        }

    //    case MsgCat::Match:
    //        break;
    //    }
    //    break;
    //default:
    //    break;
    //}
}



}  // namespace mangl::mmp



