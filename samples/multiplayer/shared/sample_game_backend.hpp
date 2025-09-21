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

#include <apeal/singleton.hpp>

namespace sample::mp {

class SampleGameServer: public mmp::GameServer<SampleGamePacket>, public apl::Singleton<SampleGameServer> {
public:
    void setup(mmp::MpGameParams params) {
        params.di = {
            .alias = "smpserver",
            .proto = ProtocolName,
            .protv = ProtocolVersion,
            .appn = "sample_mp_server",
            .appv = "0.0.1",
        };

        params.matchCreate = mmp::MatchCreateStrategy::AutoCreate,
        params.matchLaunch = mmp::MatchLaunchStrategy::AutoLaunch,

        params.minPlayers = 2;
        params.maxPlayers = 2;

        params.countdown = 3;

        setupGameServer(params);
    }

    void start() {
        startGameServer();
    }

};

}  // namespace sample

