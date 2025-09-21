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

namespace mangl::mmp {

// The player information used to send data in the packet
struct MpPacketPlayer {
    MpPlayerKey key{};

    String id;
    String alias;
    String name;
    Char color{};
    Char status{};

    void update(MpPlayerCP player) noexcept {
        key = player->key;
        id = player->id;
        alias = player->alias;
        name = player->name;
        //color = player->color;
    }
};  

using MpPacketPlayerContainer = std::vector<MpPacketPlayer>;

}  // namespace mangl::mmp

