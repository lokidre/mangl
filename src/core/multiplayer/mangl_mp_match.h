/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_player.h"
#include "mangl_mp_player_container.h"
#include "mangl_mp_packet_player.h"
#include "mangl_mp_match_stage.h"

namespace mangl::mmp {


struct MpMatch {
    MpMatchKey matchKey{};
    String matchId{};

    // Status
    MatchStage stage{};
    bool isLaunched() const noexcept {
        using enum MatchStage;
        return Launched == stage || Countdown == stage || Paused == stage || Playing == stage || Finished == stage;
    }

    int countdownCurrent{};
    Timestamp countdownTimestamp{};


    std::vector<MpPlayerP> players;

    GameInstanceBaseP gameInstance{};

    MpPacketPlayerContainer packetPlayers;

    void updatePacketPlayers() {
        packetPlayers.resize(players.size());

        auto ppIt = packetPlayers.begin();

        for (auto player: players) {
            auto& pp = *ppIt++;
            pp.update(player);
        }

    }
};


}  // namespace mangl::mmp


