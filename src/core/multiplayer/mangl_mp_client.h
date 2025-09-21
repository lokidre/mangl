/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_client_base.h"

namespace mangl::mmp {

template <class PacketT>
class GameClient: public GameClientBase {
    using Packet = PacketT;
    using PacketP = Packet*;
    using PacketCP = const Packet*;

    using OnPacket = std::function<void (PacketCP)>;

    PacketT recvPacket_{};
    PacketT outPacket_{};


public:
    //
    // Packet sending
    //
    auto beginPacket(MsgTyp type, MsgCat cat, MsgOrd ord) {
        auto packet = static_cast<PacketT*>(packetPrepare(type, cat, ord));
        return packet;
    }

    auto beginCommand(MsgCat cat, MsgOrd ord) { return beginPacket(MsgTyp::Command, cat, ord); }
    auto beginCommandMatch(MsgOrd ord) { return beginCommand(MsgCat::Match, ord); }
    auto beginCommandGame(MsgOrd ord) { return beginCommand(MsgCat::Game, ord); }


    void sendPacket(PacketP packet) { packetSend(packet); }


    //
    // More specific packet sending
    //
    void sendMatchReady() {
        sendPacket(beginCommandMatch(MsgOrd::MatchReady));
    }


    //
    // Packet processing
    //
    OnPacket onSystem{};
    OnPacket onMatch{};
    OnPacket onGame{};


protected:
    MpPacketP onPacketUnpack(MpPeerP peer, DataParamA data) override {

        switch (peer->format) {
            using enum net::DataTransferFormat;
        case Packet:
            packetLoader_.load(data.data(), data.size());
            unpack(packetLoader_, &recvPacket_);
            break;
        case Json:
            jsonParser_.parse(data.str(), &jsonPacket_);
            unpack(jsonPacket_, &recvPacket_);
            break;
        case Yaml:
            yamlParser_.parse(data.str(), &yamlPacket_);
            unpack(yamlPacket_, &recvPacket_);
            break;
        default:
            MDebugTrap();
            break;
        }

        return &recvPacket_;
    }

    MpPacketP onPacketPrepare() override {
        clear(&outPacket_);
        return &outPacket_;
    }

    DataParam onPacketBuild(MpPeerP peer, MpPacketP mpack) override {
        auto packet = static_cast<PacketT*>(mpack);
        DataParam data{};

        switch (peer->format) {
            using enum net::DataTransferFormat;
        case Packet:
            pack(*packet, &packetPacker_);
            data = packetPacker_.makeStdSpan();
            break;
        case Json:
            pack(*packet, &jsonBuilder_);
            data = jsonBuilder_.str();
            break;
        case Yaml:
            pack(*packet, &yamlBuilder_);
            data = yamlBuilder_.str();
            break;
        default:
            MDebugTrap();
            break;
        }

        return data;
    }


    void onProcessPacket(MpPeerCP peer, MpPacketCP mpack) override {
        auto packet = static_cast<PacketCP>(mpack);

        if (packet->isActionGame() || packet->isEventGame()) 
            if (onGame) return onGame(packet);

        if (packet->isActionMatch()) 
            if (onMatch) return onMatch(packet);

        if (packet->isActionSystem())
            if (onSystem) return onSystem(packet);
    }

    void onProcessClearCallbacks() override {
        onMatch = {};
        onGame = {};
    }

};




}  // namespace mangl::mmp
