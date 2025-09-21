/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_network_base.h"
#include "mangl_network_discovery_client.h"

#if MANGL_IXWEBSOCKET

namespace mangl::net {

class IxWebSocketClient: public NetworkBaseClient {
    NetworkDiscoveryClient discoveryClient_;
    PeerKey discoveredPeerKey_{};

    void* ixWebSocket_{};

    void processRecvMessage(const void* pmsg);

protected:
    void doStartDiscovery(DiscoveryInfoA di) override;
    void doStopDiscovery() override { discoveryClient_.stopDiscovery(); }
    bool doIsDiscovering() override { return discoveryClient_.isDiscovering(); }

    void doConnect() override;
    void doDisconnect() override;

    void doSend(NetworkDataSendParamsA params) override;
};

}  // namespace mangl::net

#endif

