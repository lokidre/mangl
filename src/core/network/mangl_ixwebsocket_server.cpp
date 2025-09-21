/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_ixwebsocket_server.h"

#include "../mangl_framework.h"
#include "mangl_network_discovery_server.h"

#if MANGL_IXWEBSOCKET

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <ixwebsocket/IXWebSocketServer.h>

#include <apeal/console.hpp>


namespace mangl::net {

namespace ix_impl_ {
using IxWss = ix::WebSocketServer;
using IxWssP = IxWss*;
} // namespace impl

using namespace ix_impl_;

void IxWebSocketServer::doStart() {

    ix::initNetSystem();

    if (ixWebSocketServer_) {
        auto wss = static_cast<IxWssP>(ixWebSocketServer_);
        delete wss;
        ixWebSocketServer_ = nullptr;
    }

    auto wss = new IxWss(startParams_.port, startParams_.host);
    ixWebSocketServer_ = wss;
        //impl_ = std::make_unique<IxWebSocketServerImpl>(port_, host_);

    //auto wss = static_cast<IxWssP>(ixWebSocketServer_);


    ix::SocketTLSOptions opt;

    opt.caFile = "NONE";
    opt.tls = false;
    // opt.certFile = "~/sdk/websocket_server_ssl_lib/certificate.pem";
    // opt.keyFile =  "~/sdk/websocket_server_ssl_lib/key.pem";
    // opt.caFile = "~/sdk/websocket_server_ssl_lib/chain.pem";
    // opt.tls = true;

    //impl_->di = di;

    //auto& ws = impl_->websocketServer;

    wss->setTLSOptions(opt);


    //wss->enablePong();
    wss->disablePong();
    //wss->disablePerMessageDeflate();


    using IxMessageArg = const ix::WebSocketMessagePtr&;
    using IxStateArg = const std::shared_ptr<ix::ConnectionState>&;

    wss->setOnClientMessageCallback([this](IxStateArg state, ix::WebSocket& ws, IxMessageArg msg) {
        processRecvMsg(state.get(), &ws, msg.get());
    });

    auto res = wss->listenAndStart();
}


void IxWebSocketServer::processRecvMsg(const void* pstate, void* pws, const void* pmsg) {
    using WsMsg = ix::WebSocketMessageType;

    auto state = static_cast<const ix::ConnectionState*>(pstate);
    auto ws = static_cast<ix::WebSocket*>(pws);
    auto msg = static_cast<const ix::WebSocketMessage*>(pmsg);


    //ADebugPrint("Message! Id: %s", state->getId());
    int peerKey = apl::parseInt(state->getId()) + 1;
    auto connIt = connections_.find(peerKey);

    IxConnectionInfo* connInfo = connIt != connections_.end() ? &connIt->second : nullptr;
    PeerP peer = connInfo ? &connInfo->peer : nullptr;

    switch (msg->type) {
    case WsMsg::Open:
        //ADebugPrint("Msg Open!");
        if (!connInfo) {
            IxConnectionInfo pi{};
            pi.websocket = ws;
            pi.peer.key = peerKey;
            connections_.emplace(peerKey, pi);
        }
        break;

    case WsMsg::Close:
        if (peer) {
            events_.pushUpdateState(peer, PeerState::Disconnected);
            connections_.erase(connIt);
        }
        break;

    case WsMsg::Message:
        if (!connInfo)
            break;

        if (!connInfo->diReceived) {
            peer->di.parseData(msg->str);
            MDebugTrapAssert(peer->di.alias);

            connInfo->diReceived = true;

            events_.pushUpdateFound(peer, msg->str);
            events_.pushUpdateState(peer, PeerState::Connected);

            ws->send(startParams_.di.build(), true);
            break;
        } 
        
        events_.pushData(peerKey, msg->str);
        break;

    default: break;
    }

}


void IxWebSocketServer::doStartAdvertising() {
    discoveryServer_.startAdvertising({
        .di = startParams_.di,
        .scheme = startParams_.scheme,
        .port = startParams_.port,
    });

    isAdvertising_ = true;
}

void IxWebSocketServer::doDisconnect(PeerKey pk) {

    auto peerIt = connections_.find(pk);

    if (peerIt == connections_.end()) {
        ADebugTrap();
        return;
    }

    auto& peer = peerIt->second;

    auto ws = (ix::WebSocket*)peer.websocket;

    connections_.erase(peerIt);
}

void IxWebSocketServer::doShutdown() {
    auto wss = static_cast<IxWssP>(ixWebSocketServer_);
    if (!wss)
        return;


    // TODO: send disconnect signal to all the clients
    wss->stop();
    delete wss;
    ixWebSocketServer_ = nullptr;
}

void IxWebSocketServer::doSend(NetworkDataSendParamsA params) {

    auto peerIt = connections_.find(params.pk);

    if (peerIt == connections_.end()) {
        ADebugTrap();
        return;
    }

    auto& peer = peerIt->second;

    auto ws = static_cast<ix::WebSocket*>(peer.websocket);
    const auto& data = params.data;

    ws->send({(const char*)data.data(), data.size()}, true);
}


} // namespace mangl::net


#endif
