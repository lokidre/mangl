/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "sample_mp_protocol.hpp"

#include "sample_game_packet.hpp"
#include "sample_game_packet_gen.hpp"
#include "sample_game_packet_gen_yaml.hpp"
#include "sample_game_packet_gen_json.hpp"


#include <mangl/multiplayer.h>
#include <mangl/app.h>
#include <mangl/framework.h>
#include <mangl/environment.h>


namespace sample::mp {

class SampleGameNpc: public mmp::GameClient<SampleGamePacket> { 
    using Packet = SampleGamePacket;
    using PacketP = SampleGamePacketP;
    using PacketCP = SampleGamePacketCP;



    bool stateInitialized_{};
    mmp::PacketKey npcElementKey_{};
    Timestamp actionTimestamp_{};

    void updateGameState(PacketCP packet) {
        if (!stateInitialized_) {
            stateInitialized_ = true;

            for (auto& el: packet->elements) {
                if (el.player == getPlayerKey())
                    npcElementKey_ = el.key;
            }
        }

        //if (!state_.guiCreated) {
        //    createGameStateGui(packet);
        //    return;
        //}

        //for (auto& pel: packet->elements) {
        //    auto it = state_.elements(
        //}
    }




public:
    SampleGameNpc() {
        onMatch = [this](auto packet) {
            //auto client = SampleGameFrontend::instance();

            if (packet->isMatchLaunch())  return sendMatchReady();
        };

        onGame = [this](auto packet) {
            if (packet->isGameState()) return updateGameState(packet);
        };

        onTimer = [this](auto timestamp) {
            if (!stateInitialized_)
                return;

            if (actionTimestamp_ == 0) {
                actionTimestamp_ = timestamp + 2;
                return;
            }

            if (timestamp >= actionTimestamp_) {
                actionTimestamp_ = timestamp + 2;

                auto packet = beginCommandGame(MsgOrd::GameElement);

                packet->element.key = npcElementKey_;
                packet->element.player = getPlayerKey();
                packet->element.rect = M::randomRect({10, 10}, {300, 500}, {20, 20}, {30, 30});

                sendPacket(packet);

                //M::random.range(
            }
        };

    }


protected:
    void onSetup(mmp::MpGameParams* params) {
        params->connectionType = net::ConnectionType::Local;

        params->di = {
            .alias = "npc",
            .proto = ProtocolName,
            .protv = ProtocolVersion,
            .appn = "sample_npc",
            .appv = "0.0.1",
        };

        params->di.setParam("npc", "1");
        params->di.setFormat(net::DataTransferFormat::Packet);

        params->debugVerboseAll = true;

        status_.isNpc = true;
    }


};

}  // namespace sample
