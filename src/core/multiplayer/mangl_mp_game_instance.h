/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_game_instance_base.h"
#include "mangl_mp_server.h"

namespace mangl::mmp {



template <class PacketT>
class GameInstance: public GameInstanceBase {

protected:
    using Packet = PacketT;
    using PacketP = Packet*;
    using PacketCP = const Packet*;

    using Match = MpMatch;
    using MatchP = MpMatchP;
    using MatchCP = MpMatchCP;

    using GameServerP = GameServer<Packet>*;


protected:
    GameServerP server_{};
    MatchP match_{};
   

protected:
    auto beginAction(MessageOrder ord) {
        auto packet = packetPrepareActionGame(ord);
        return packet;
    }

    auto beginAction(MpPlayerP player, MessageOrder ord) {
        auto packet = packetPrepareActionGame(ord);
        packet->to = player->key;
        packet->rtToPlayer = player;
        return packet;
    }


    void send(PacketP packet) {
        if (packet->to) {
            server_->packetPush(packet->rtToPlayer, packet);
        } else {  // Send to all players
            for (auto player: match_->players) {
                server_->packetPush(player, packet);
            }
        }
    }


    //
    // More detailed game packets
    //
    auto beginGameState() { return beginAction(MessageOrder::GameState); }
    auto beginGameField() { return beginAction(MessageOrder::GameField); }


protected:
    virtual void onMatchCreate(MpMatchP match) {}
    virtual void onMatchLaunch(MpMatchP match, PacketP packet) {}
    virtual void onMatchBegin(Timestamp timestamp) {}
    virtual void onMatchEnd() {}

    virtual void onTimer(Timestamp timestamp) {}

    virtual void onAction(PacketCP packet) {}
    virtual void onCommand(MpPlayerCP player, PacketCP packet) {}



private:
    PacketT* packetPrepareActionGame(MessageOrder ord) {
        auto mppacket = server_->packetPrepare(MessageType::Action, MessageCategory::Game, ord);
        auto packet = static_cast<PacketP>(mppacket);
        return packet;
    }

private:
    void onServerInstance(GameServerBaseP server) override {
        server_ = static_cast<GameServerP>(server);
    }

    void onServerMatchLaunch(MpMatchP match, MpPacketP packet) override {
        match_ = match;
        onMatchLaunch(match, static_cast<PacketP>(packet));
    }

    void onServerMatchBegin(Timestamp timestamp) override { onMatchBegin(timestamp); }
    void onServerTimer(Timestamp timestamp) override { onTimer(timestamp); }

    void onServerActionPacket(MpPacketCP packet) override {
        onAction(static_cast<PacketCP>(packet));
    }

    void onServerCommandPacket(MpPlayerCP player, MpPacketCP packet) override {
        onCommand(player, static_cast<PacketCP>(packet));
    }

    void onServerMatchEnd() override {
        onMatchEnd();
    }



};

}  // namespace mangl::mmp

