/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_ixwebsocket_client.h"

#include "mangl_network_discovery_client.h"

#include <apeal/parse/split.hpp>
#include <apeal/parse/vector.hpp>

#if MANGL_IXWEBSOCKET

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <ixwebsocket/IXWebSocketServer.h>

namespace mangl::net {

void IxWebSocketClient::doStartDiscovery(DiscoveryInfoA di) {
    ix::initNetSystem();

    discoveredPeerKey_ = {};

    discoveryClient_.startDiscovery({
        .di = di,
        .cb = [this](DiscoveryInfoA di) {
            auto data = di.build();
            discoveredPeerKey_ = generatePeerKey();
            events_.pushFound(discoveredPeerKey_, data);
            discovered_ = true;
        },
    });
}


void IxWebSocketClient::doConnect() {
    ix::initNetSystem();

    if (!ixWebSocket_) {
        ixWebSocket_ = new ix::WebSocket();
    }

    auto ws = static_cast<ix::WebSocket*>(ixWebSocket_);

    ix::SocketTLSOptions opt;
    opt.caFile = "NONE";
    ws->setTLSOptions(opt);

    ws->setUrl(connectParams_.url);

    // Optional heart beat, sent every 45 seconds when there is not any traffic
    // to make sure that load balancers do not kill an idle connection.
    ws->setPingInterval(45);
    //ws->disablePong();
    //ws->disableAutomaticReconnection();

    //
    // impl_->setDiscoveryInfo(di);
    //di_ = di;

    // Per message deflate connection is enabled by default. You can tweak its parameters or disable
    // it
    //ws.disablePerMessageDeflate();

    ws->setOnMessageCallback([this](const auto& msg) {
        processRecvMessage(msg.get());
    });

    hostPeerInfo_.key = discoveredPeerKey_ ? discoveredPeerKey_ : generatePeerKey();
    auto status = ws->connect(connectParams_.timeout);

    if (!status.success) {
        events_.pushError(hostPeerInfo_.key, 0, status.errorStr);
        ws->stop();
        return;
    }

    // Send the discovery info
    ws->send(connectParams_.di.build());

    ws->start();
}

void IxWebSocketClient::processRecvMessage(const void* pmsg) {
    auto msg = static_cast<const ix::WebSocketMessage*>(pmsg);

    using WsMsg = ix::WebSocketMessageType;

    PeerP peer = hostPeerInfo_.key ? &hostPeerInfo_ : nullptr;
    //auto& di = peer->di;


    switch (msg->type) {

    case WsMsg::Error:
        MDebugPrint("WS Error: %s", msg->errorInfo.reason);
        break;

    case WsMsg::Open:
        MDebugPrint("WS Open! prot:%s, uri: %s", msg->openInfo.protocol, msg->openInfo.uri);
        break;

    case WsMsg::Close:
        MDebugPrint("WS Close.");
        discoveryInfoReceived_ = false;
        discovered_ = false;

        if (peer) {
            events_.pushUpdateState(peer, PeerState::Disconnected);
        }
        break;

    case ix::WebSocketMessageType::Fragment:
        MDebugPrint("Msg Fragment");
        break;

    case ix::WebSocketMessageType::Message:
        //MDebugPrint("Msg Message");
        if (!peer)
            break;


        if (!discoveryInfoReceived_) {
            auto& di = peer->di;

            di.parseData(msg->str);
            if (di.alias.empty()) {
                MDebugPrint("Invalid Discovery Info");
                return;
            }

            discoveryInfoReceived_ = true;

            MDebugPrint("Discovery: %d, %s", peer->key, di.alias);

            connected_ = true;

            //peer->player.alias = peer->player.name = di.alias;
            if (!discovered_) {
                events_.pushUpdateFound(peer, msg->str);
            }

            events_.pushUpdateState(peer, PeerState::Connected);
            break;
        } 
        
        
        if (!connected_) {
            processConnected();

            if (discovered_) {
                events_.pushUpdateFound(peer, msg->str);
            }

            events_.pushUpdateState(peer, PeerState::Connected);
        }

        events_.pushData(peer->key, msg->str);
        break;

    default:
        break;
    }

}



void IxWebSocketClient::doDisconnect() {
    if (ixWebSocket_) {
        auto ws = static_cast<ix::WebSocket*>(ixWebSocket_);
        ws->close();
        delete ws;
        ixWebSocket_ = nullptr;
    }

    hostPeerInfo_.key = 0;
}


void IxWebSocketClient::doSend(NetworkDataSendParamsA params) {
    if (!connected_ || !ixWebSocket_) {
        MDebugTrap();
        return;
    }

    auto ws = static_cast<ix::WebSocket*>(ixWebSocket_);
    const auto& data = params.data;
    ws->send({(const char *)data.data(), data.size()}, true);
}

} // namespace mangl::net


#endif
