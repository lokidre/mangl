/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "./multiplayer/mangl_mp_client.h"
#include "./multiplayer/mangl_mp_server.h"
#include "./multiplayer/mangl_mp_game_instance.h"

namespace mangl {

struct MultiplayerInvite {
    mmp::MpPlayer player;
    uint32_t attributes;
    uint64_t group;
};

struct Multiplayer: public apl::Singleton<Multiplayer> {
    Signal<const MultiplayerInvite*> inviteReceivedSignal;
};

}  // namespace mangl

