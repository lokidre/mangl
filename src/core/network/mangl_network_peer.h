/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_network_discovery_info.h"

namespace mangl::net {

struct Peer {
    PeerKey key{};
    DiscoveryInfo di{};
    PeerState state{};

    bool isConnected() const noexcept { return PeerState::Connected == state; }
    void setDisconnected() noexcept { state = PeerState::Disconnected; }
    void setConnected() noexcept { state = PeerState::Connected; }
};

using PeerP = Peer*;
using PeerCP = const Peer*;

}  // namespace mangl
