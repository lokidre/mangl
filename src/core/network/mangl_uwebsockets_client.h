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

#if 0 //MANGL_UWEBSOCKETS

namespace mangl::net {


struct UWebSocketsClientImpl;

class UWebSocketsClient: public NetworkBase {
    std::unique_ptr<UWebSocketsClientImpl> impl_;

    NetworkDiscoveryClient discoveryClient_;

    Peer hostPeerInfo_{};
    int currentHostPeerKey_{1};

    void processRecvMessage(const void* pmsg);

public:
    UWebSocketsClient();
    ~UWebSocketsClient() override;

    void startDiscovery(DiscoveryInfoA di) override;
    bool isDiscovering() const override;
    void stopDiscovery() override;

    void connect(StrView url, DiscoveryInfoA di, PeerKey pk = 0, int timeout = 30) override;
    void disconnect() override;

    void shutdown() override;

    void send(DataParamA data, bool unreliable = false) override;

protected:
    void onTimerPostfix(Timestamp timestamp) override;

};

}  // namespace mangl::net

#endif

