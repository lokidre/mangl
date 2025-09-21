/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_server.h"

namespace mangl::mmp {


void GameServerBase::debugPrintStats() {
    if (peers_.size() == debugLastPeersCount_ 
        && players_.size() == debugLastPlayersCount_
        && matches_.size() == debugLastMatchesCount_
        ) 
        return;

    String text = "Stats: ";

    text += apl::fmt("Peers: %d", peers_.size());
    text += apl::fmt(", Players: %d", players_.size());
    text += apl::fmt(", Matches: %d", matches_.size());

    if (onDebugMessage) onDebugMessage(text);
    if (onDebugMessageMatch) onDebugMessageMatch(text);


    debugLastPeersCount_ = peers_.size();
    debugLastPlayersCount_ = players_.size();
    debugLastMatchesCount_ = matches_.size();
}

}  // namespace mangl::mmp

