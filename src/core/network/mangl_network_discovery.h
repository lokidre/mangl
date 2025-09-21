/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"
#include "../mangl_time.h"


#include "mangl_network_discovery_info.h"

//#include "../mangl_network.h"

#include <apeal/socket.hpp>


namespace mangl::net {

class NetworkDiscoveryBase {
protected:
#ifdef MANGL_NET_DISCOVERY_PORT
    int discoveryPort_ = MANGL_NET_DISCOVERY_PORT;
#else
    int discoveryPort_ = 14300;
#endif

    apl::Socket socket_;

    TimerHandler timerHandler_;
};

}  // namespace mangl::net
