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

namespace mangl::net {


struct NetworkAdvertisingParams {
    DiscoveryInfo di;
    String scheme;
    unsigned port;
};

using NetworkAdvertisingParamsA = const NetworkAdvertisingParams&;

class NetworkDiscoveryServer: public NetworkDiscoveryBase {
    using Self = NetworkDiscoveryServer;

    NetworkAdvertisingParams params_{};

    Timestamp advertizeTimestamp_{};
    Timestamp advertizingPeriod_{5};

    bool isAdvertizing_{};
    String broadcastAddress_;
    String advertisingData_;

    struct sockaddr_in servaddr_{};

    void onTimer(Timestamp timestamp);

public:
    void startAdvertising(NetworkAdvertisingParamsA params);
    bool isAdvertising() const noexcept { return isAdvertizing_; }
    void stopAdvertising();
};

} // namespace mangl::net