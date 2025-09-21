/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_network_packet.h"
#include "../mangl_multiplayer.h"

namespace mangl::net {

struct Event {
    Packet packet;
    PeerKey peerKey{};
    PeerState peerState{};

    enum class Type {
        Invalid = 0, Packet, Found, State, Error,
    } type{};

    bool pending{};

    void reset(Type t = Type::Invalid) {
        type = t;
        packet.valid = {};
        peerState = {};
    }
};

}  // namespace mangl::net
