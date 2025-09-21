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

#if MANGL_UWEBSOCKETS

#include <thread>

namespace mangl::net {


class UWebSocketsServer: public NetworkBaseServer {
    using Self = UWebSocketsServer;

    void* uwsApp_{};
    void* uwsListenSocket_{};

    struct ConnectionContext: public Peer {
        bool diReceived{};
        void* ws{};
    };

    Mutex peersMutex_;
    using Lock = std::scoped_lock<Mutex>;
    PeerContainer<ConnectionContext> peers_;
    std::vector<ConnectionContext*> disconnectedPeers_;
    std::atomic_bool haveDisconnectedPeer_{};

    ConnectionContext* getPeer(PeerKey pk) noexcept {
        Lock lock{peersMutex_};
        return peers_.get(pk);
    }

    void peersMaintenance() {
        if (!haveDisconnectedPeer_) return;
        haveDisconnectedPeer_ = false;

        Lock lock{peersMutex_};
        for (auto p: disconnectedPeers_) {
            if (p->state == PeerState::Disconnected)
                peers_.erase(p);
        }
        disconnectedPeers_.clear();
    }

    //bool compress_{};

    NetworkDiscoveryServer discoveryServer_;

    std::jthread connectionThread_;
    //void connectionThreadFunc();


    template <class AppT, class SocketT>
    void connectionThreadFunc();

    template <class AppT, class SocketT>
    void disconnectBody(PeerKey pk);

    template <class AppT, class SocketT>
    void shutdownBody();

    template <class AppT, class SocketT>
    void sendBody(NetworkDataSendParamsA params);

    template <class AppT, class SocketT>
    void stopListeningBody();

public:
    virtual ~UWebSocketsServer() {
        int a =0;
    }

protected:
    void doStart() override;

    //void onStartListening() override;
    void doStopListening() override;


    void doStartAdvertising() override;
    void doStopAdvertising() override { discoveryServer_.stopAdvertising(); }

    void doSend(NetworkDataSendParamsA params) override;

    void doDisconnect(PeerKey pk) override;
    void doShutdown() override;
};


}  // namespace mangl::net

#endif
