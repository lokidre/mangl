/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_uwebsockets_server.h"

#include "mangl_network_discovery_server.h"

#include "../mangl_framework.h"

#if MANGL_UWEBSOCKETS

#include "../3rdparty/uwebsockets/App.h"


namespace mangl::net {

namespace impl_ {

struct UWebSocketPerSocketData {
    PeerKey peerKey{};
};


using SslApp = uWS::SSLApp;
using SslWS = uWS::WebSocket<true, true, UWebSocketPerSocketData>;

using NoSslApp = uWS::App;
using NoSslWS = uWS::WebSocket<false, true, UWebSocketPerSocketData>;

}  // namespace impl_

using namespace impl_;



void UWebSocketsServer::doStart() {
    if (startParams_.ssl) {
        connectionThread_ = std::jthread{[this] { connectionThreadFunc<SslApp, SslWS>(); }};
    } else {
        connectionThread_ = std::jthread{[this] { connectionThreadFunc<NoSslApp, NoSslWS>(); }};
    }
}

template <class AppT, class SocketT>
void UWebSocketsServer::connectionThreadFunc() {

    auto app = new AppT();

    uwsApp_ = app;

    app->template ws<UWebSocketPerSocketData>("/*", {
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024 * 1024,
        //.idleTimeout = 16,
        .maxBackpressure = 1 * 1024 * 1024,
        .closeOnBackpressureLimit = false,
        .resetIdleTimeoutOnSend = false,
        .sendPingsAutomatically = true,

        .upgrade = nullptr,

        .open = [this](auto* ws) {
            auto userData = ws->getUserData();

            peersMaintenance();

            userData->peerKey = generatePeerKey();


            Lock lock{peersMutex_};
            auto peer = peers_.create(userData->peerKey, {});
            peer->diReceived = false;
            peer->ws = ws;
            peer->state = PeerState::Found;
        },

        .message = [this](auto* ws, std::string_view message, uWS::OpCode opCode) {
            //MDebugPrint("ws::msg::opcode: %d", int(opCode));

            auto userData = ws->getUserData();
            auto peer = getPeer(userData->peerKey);
            if (!peer) return;

            if (!peer->diReceived) {
                {
                    Lock lock{peersMutex_};
                    peer->di.parseData(message);
                }
                MDebugTrapAssert(peer->di.alias);

                peer->diReceived = true;

                events_.pushUpdateFound(peer, message);
                events_.pushUpdateState(peer, PeerState::Connected);

                //ws->send(di_.build(), true);
                ws->send(startParams_.di.build(), uWS::BINARY, startParams_.compress);
                return;
            } 

            events_.pushData(peer->key, message);
        },

        //.drain = [](auto* ws) {
        //    /* Check ws->getBufferedAmount() here */
        //},
        //.ping = [](auto* ws, std::string_view) {},
        //.pong = [](auto* ws, std::string_view) {},
        .close = [this](auto* ws, int code, std::string_view message) {
            auto userData = ws->getUserData();
            auto peer = getPeer(userData->peerKey);
            if (!peer) return;
            events_.pushUpdateState(peer, PeerState::Disconnected);
            haveDisconnectedPeer_ = true;
        },
    });


    app->listen("", startParams_.port, [this](auto* listen_socket) {
        //int a = 0;
        if (listen_socket) {
            uwsListenSocket_ = listen_socket;
            //std::cout << "Listening on port " << 9001 << std::endl;
        }
    });

    app->run();

    delete app;


    //int a = 0;
}


template <class AppT, class SocketT>
void UWebSocketsServer::sendBody(NetworkDataSendParamsA params) {
    auto peer = getPeer(params.pk);

    if (!peer) {
        MDebugTrap();
        return;
    }

    auto app = static_cast<AppT*>(uwsApp_);

    String sdata{params.data.strview()};

    app->getLoop()->defer([this, peer, sdata] {
        auto ws = static_cast<SocketT*>(peer->ws);
        ws->send(sdata, uWS::BINARY, startParams_.compress);
    });

}


void UWebSocketsServer::doSend(NetworkDataSendParamsA params) {

    if (startParams_.ssl) {
        sendBody<SslApp, SslWS>(params);
    } else {
        sendBody<NoSslApp, NoSslWS>(params);
    }

}



void UWebSocketsServer::doStartAdvertising() {
    discoveryServer_.startAdvertising({
        .di = startParams_.di,
        .scheme = startParams_.scheme,
        .port = startParams_.port,
    });
}

template <class AppT, class SocketT>
void UWebSocketsServer::disconnectBody(PeerKey pk) {

    auto peer = getPeer(pk);

    if (!peer) {
        ADebugTrap();
        return;
    }


    auto app = static_cast<AppT*>(uwsApp_);

    app->getLoop()->defer([this, peer] {
        auto ws = static_cast<SocketT*>(peer->ws);
        
        ws->end();
        ws->close();

        Lock lock{peersMutex_};
        peers_.erase(peer);
    });
}


void UWebSocketsServer::doDisconnect(PeerKey pk) {
    if (startParams_.ssl) {
        disconnectBody<SslApp, SslWS>(pk);
    } else {
        disconnectBody<NoSslApp, NoSslWS>(pk);
    }
}


template <class AppT, class SocketT>
void UWebSocketsServer::shutdownBody() {
    auto app = static_cast<AppT*>(uwsApp_);
    app->getLoop()->defer([this, app] {

        for (auto p: peers_) {
            if (p->state == PeerState::Disconnected)
                continue;

            auto ws = static_cast<SocketT*>(p->ws);
            ws->end();
            ws->close();
        }

        app->close();

        //Lock lock{peersMutex_};
        peers_.clear();
    });
}


void UWebSocketsServer::doShutdown() {
    if (!uwsApp_) return;

    if (startParams_.ssl) {
        shutdownBody<SslApp, SslWS>();
    } else {
        shutdownBody<NoSslApp, NoSslWS>();
    }

    if (connectionThread_.joinable()) 
        connectionThread_.join();

    uwsApp_ = nullptr;
}



template <class AppT, class SocketT>
void UWebSocketsServer::stopListeningBody() {
    auto app = static_cast<AppT*>(uwsApp_);

    app->getLoop()->defer([this] {
        auto listenSocket = static_cast<us_listen_socket_t*>(uwsListenSocket_);
        auto listenSSL = 0;
        us_listen_socket_close(listenSSL, listenSocket);
    });
}

void UWebSocketsServer::doStopListening() {
    if (startParams_.ssl) {
        stopListeningBody<SslApp, SslWS>();
    } else {
        stopListeningBody<NoSslApp, NoSslWS>();
    }
}




} // namespace mangl::net


#endif
