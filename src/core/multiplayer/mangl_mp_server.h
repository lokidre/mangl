/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_server_base.h"

namespace mangl::mmp {

template <class PacketT>
class GameServer: public GameServerBase {
    using Packet = PacketT;
    using PacketP = Packet*;
    using PacketCP = const Packet*;

    PacketT recvPacket_{};
    PacketT outPacket_{};

protected:
    PacketP beginGameState() {
    }

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

    MpPacketP onPacketPrepare() override { return &outPacket_; }

    DataParam onPacketBuild(MpPeerP peer, MpPacketP mpack) override {
        auto packet = static_cast<PacketP>(mpack);
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
    }

    void onProcessClearCallbacks() override {
    }


};



}  // namespace mangl::mmp

