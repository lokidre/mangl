/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "sample_game_packet.hpp"

#include <mangl/multiplayer.h>

namespace sample::mp {

struct SampleGameState {
    mmp::MpPacketElementsContainer elements;

    bool gameBegan{};
    Timestamp gameBeginTimestamp{};
};

class SampleGameInstance: public mmp::GameInstance<SampleGamePacket> {
    using Self = SampleGameInstance;
    using Base = mmp::GameInstance<SampleGamePacket>;

    using Base::MatchP;


    SampleGameState state_;


    void createGame() {
        unsigned currentKey = 1;

        std::vector<uint32_t> colors = {M::Red, M::Yellow, M::Blue, M::Green};
        auto colorIndex = 0;

        // Create idle element
        auto& elements = state_.elements;
        elements.push_back({
            .key = currentKey++,
            .color = colors[colorIndex++],
            .rect = {10, 20, 30, 30},
        });

        for (auto player: match_->players) {
            elements.push_back({
                .key = currentKey++,
                .player = player->key,
                .color = colors[colorIndex++],
                .rect = {30, 50, 20, 20},
            });
        }
    }

    void sendGameState() {
        auto packet = beginGameState();
        packet->elements = state_.elements;
        send(packet);
    }

protected:

    void onMatchLaunch(MatchP match, PacketP packet) override {
        createGame();
        sendGameState();
    }

    void onMatchBegin(Timestamp timestamp) override {
        state_.gameBegan = true;
        state_.gameBeginTimestamp = timestamp;
    }

    void onTimer(Timestamp timestamp) override {
        if (!state_.gameBegan) return;
    }



};

}  // namespace sample


