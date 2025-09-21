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
#include "mangl_network_discovery_server.h"

#if MANGL_IXWEBSOCKET

namespace mangl::net {

class IxWebSocketServer: public NetworkBaseServer {
    void* ixWebSocketServer_{};

    NetworkDiscoveryServer discoveryServer_;

    struct IxConnectionInfo {
        bool diReceived{};
        void* websocket{};
        Peer peer;
    };

    std::unordered_map<PeerKey, IxConnectionInfo> connections_;

    void processRecvMsg(const void* pstate, void* pws, const void* pmsg);


protected:
    void doStart() override;

    void doStartAdvertising() override;
    void doStopAdvertising() override { discoveryServer_.stopAdvertising(); }

    void doSend(NetworkDataSendParamsA params) override;

    void doDisconnect(PeerKey pk) override;
    void doShutdown() override;
};


}  // namespace mangl::net

#endif
