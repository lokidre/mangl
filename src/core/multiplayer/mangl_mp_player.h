/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_types.h"

namespace mangl::mmp {


struct MpPlayer {
    // Connection
    MpPeerP peer{};
    PeerKey peerKey{};  // Used for reconnecting players (new peer key)
//    String playerId;
//    String playerAlias;
//    String playerName;

    [[nodiscard]] bool isNpc() const noexcept { return peer->isNpc; }
    
    PeerState peerState{};
    Timestamp lastSendTimestamp{};
    Timestamp lastRecvTimestamp{};

    // Identification
    unsigned key{}; 
	String id;  // Player id
    String alias;
    String name;


    //
    // Current status
    //
    bool helloReceived{};
    bool welcomeSent{};


    bool matching{};
    MpMatchP match{};
    bool matchReadyReceived{};


    // Pinging
    bool pingPending{};
    Timestamp pingTimestamp{};
    uint64_t pingUserTime{};
};



}  // namespace mangl::mmp

