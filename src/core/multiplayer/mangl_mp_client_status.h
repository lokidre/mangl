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
#include "mangl_mp_match_stage.h"

namespace mangl::mmp {

struct MpClientStatus {

    MatchStage matchStage{};
    bool isNpc{};

    // Connection
    bool connected{};
    bool pendingConnect{};

    //bool pongReceived{};
    Timestamp pongTimestamp{};

    // Negotiation stage
    bool helloSent{};
    bool welcomeReceived{};
    MpPlayerKey playerKey{};
    String playerId;

    //
    // Match selection and joining
    //
    bool matchFound{};
    bool matchFindRequested{};

    bool matchJoined{};
    bool matchJoinRequested{};

    bool matchLaunched{};

    MpMatchKey matchKey{};
    String matchId{};
};

}  // namespace mangl::mmp

