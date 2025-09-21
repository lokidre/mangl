/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../network/mangl_network_discovery_info.h"

namespace mangl::mmp {

enum class MatchCreateStrategy {
    Unknown = 0,

    AutoCreate = 'A',  // If minPlayers are found then the match is created and the game launches
    HostJoin = 'H',  // One player creates match, others join

    Existing = 'E',  // Matches can't be created, only existing
};


enum class MatchLaunchStrategy: char {
    Unknown = 0,

    AutoLaunch = 'A',  // Once min players are connected the match launches
    Creator = 'C',  // The creator launches the match
    Player = 'P',  // The match launches when each player confirms
};

class GameServerBase;
class GameClientBase;


struct MpGameParams {

    //
    // Identification (server, client, player, etc)
    //
    net::DiscoveryInfo di;
    String nodeAlias;  // how to identify the node (short string) (player alias or server name)
    String nodeName;  // Player name or server name



    //
    // Connections. Communication
    //
    // Connection type
    net::ConnectionType connectionType{net::ConnectionType::Unknown};
    GameServerBase* localServer{};
    GameClientBase* localClient{};

    net::NetworkBaseClientP clientNetwork{};
    net::NetworkBaseServerP serverNetwork{};

    net::DataTransferFormat dataTransferFormat{};

    bool hostDiscovery{};  // Enabled or disable host advertising
    //bool hostAdvertisingMatch{};  // Should host be discoverable while playing
    String hostUrl{};





    //
    // Matchmaker params
    //
    // 
    // Match creation and launching strategies
    MatchCreateStrategy matchCreate{}; //MatchCreateStrategy::AutoMatch
    MatchLaunchStrategy matchLaunch{};
    bool singleMatch{};  // Single match only. For the servers run from phone
    //int matchPlayLimit{};  // Maximum number of matches can be played

    bool isMatchAutoCreate() const noexcept { return MatchCreateStrategy::AutoCreate == matchCreate; }
    bool isMatchAutoLaunch() const noexcept { return MatchLaunchStrategy::AutoLaunch == matchLaunch; }


    int minPlayers{};
    int maxPlayers{};
    int defPlayers{}; // default number of players

    int minNpcCount{};
    int maxNpcCount{};


    int countdown{};


//    MatchmakerParams match{};

    //
    // Debugging
    //

    // Status printing
    bool debugVerboseAll{};
    bool debugVerboseConState{};
    bool debugVerboseConData{};
    bool debugVerbosePacket{};
    bool debugVerboseMatch{};

    // Packet logging
    String debugPacketLogDir;
    bool debugPacketLogEnabled{};
};

using MpGameParamsA = const MpGameParams&;

}  // namespace mangl::mmp


