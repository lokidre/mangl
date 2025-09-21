/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_network_discovery.h"
#include "mangl_network_discovery_info.h"

namespace mangl::net {

using NetworkDiscoveredCallback = std::function<void(DiscoveryInfoA di)>;

struct NetworkDiscoveryParams {
    DiscoveryInfo di;
    NetworkDiscoveredCallback cb;
};

using NetworkDiscoveryParamsA = const NetworkDiscoveryParams&;


class NetworkDiscoveryClient: public NetworkDiscoveryBase {
    using Self = NetworkDiscoveryClient;

    NetworkDiscoveryParams params_{};

    bool isDiscovering_{};
    bool discovered_{};

private:
    void onTimer(Timestamp timestamp);


public:
    void startDiscovery(NetworkDiscoveryParamsA params);
    void stopDiscovery();

    bool isDiscovering() const noexcept { return isDiscovering_; }

    bool discovered() const noexcept { return discovered_; }
};

} // namespace mangl::net
