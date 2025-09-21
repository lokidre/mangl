/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"

#include <apeal/singleton.hpp>

namespace mangl {


struct GameCenter: public apl::Singleton<GameCenter> {
#if APEAL_OS_MAC && MANGL_GAMECENTER
    static void authenticate();
    static void launchLeaderboardGui(StrView leaderboardId);
    static void launchHomeGui();
    static void postScore(StrView leaderboardId, int score);
    static void inviteFriends();
#else  // Stubs
    static void authenticate(){}
    static void launchLeaderboardGui(StrView){}
    static void launchHomeGui(){}
    static void postScore(StrView, int){}
    static void inviteFriends(){}
#endif
};


}  // namespace mangl
