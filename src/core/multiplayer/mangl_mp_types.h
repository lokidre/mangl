/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../network/mangl_network_peer.h"
#include "../network/mangl_network_discovery_info.h"

namespace mangl::net {
//class NetworkBase;
//using NetworkBaseP = NetworkBase*;

class NetworkBaseClient;
using NetworkBaseClientP = NetworkBaseClient*;

class NetworkBaseServer;
using NetworkBaseServerP = NetworkBaseServer*;


}  // namespace mangl::net


namespace mangl::mmp {

using DiscoveryInfo = net::DiscoveryInfo;
using DiscoveryInfoA = net::DiscoveryInfoA;


using MpPlayerKey = unsigned;
using MpMatchKey = unsigned;


// Declarations
struct MpPeer;
using MpPeerP = MpPeer*;
using MpPeerCP = const MpPeer*;


struct MpPlayer;
using MpPlayerP = MpPlayer*;
using MpPlayerCP = const MpPlayer*;

struct MpMatch;
using MpMatchP = MpMatch*;
using MpMatchCP = const MpMatch*;


class GameInstanceBase;
using GameInstanceBaseP = GameInstanceBase*;


}  // namespace mangl::mmp




