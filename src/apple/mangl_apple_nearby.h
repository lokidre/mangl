/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


#include "../core/network/mangl_network_base.h"

namespace mangl::net {

class AppleNearbyClient: public NetworkBaseClient {
protected:
    void doStartDiscovery(DiscoveryInfoA di) override;
    bool doIsDiscovering() override;
    void doStopDiscovery() override;
    
    void doConnect() override;

    void doSend(NetworkDataSendParamsA params) override;
    void doDisconnect() override;
    void doShutdown() override;
};


class AppleNearbyServer: public NetworkBaseServer {
protected:
    void doStart() override;
    
    void doStartAdvertising() override;
    void doStopAdvertising() override;
    bool doIsAdvertising() override;
    
    void doSend(NetworkDataSendParamsA params) override;
    
    void doDisconnect(PeerKey pk) override;
    void doShutdown() override;
};


}  // namespace mangl::net
