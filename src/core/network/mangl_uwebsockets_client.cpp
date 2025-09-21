/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_uwebsockets_client.h"

#include "mangl_network_discovery_client.h"

#include <apeal/parse/split.hpp>
#include <apeal/parse/vector.hpp>

#if 0 // MANGL_UWEBSOCKETS

#include "../3rdparty/uwebsockets/App.h"


namespace mangl::net {

struct PerSocketData {
    /* Fill with user data */
};


struct UWebSocketsClientImpl {
    uWS::App* app{};
    //ix::WebSocket webSocket;
    DiscoveryInfo di;
    //NetworkDiscoveryClient discoveryClient;
};


UWebSocketsClient::UWebSocketsClient() = default;
UWebSocketsClient::~UWebSocketsClient() = default;

void UWebSocketsClient::startDiscovery(DiscoveryInfoA di) {
    //ix::initNetSystem();

    //if (!impl_)
    //    impl_ = std::make_unique<UWebSocketsClientImpl>();

    discoveryClient_.startDiscovery({
        .di = di,
        .cb = [this](DiscoveryInfoA di) {
            auto data = di.build();
            events_.pushFound(currentHostPeerKey_, data); //{data.data(), data.size()});
            discovered_ = true;
        },
    });

    isDiscovering_ = true;
    discoveryInfoReceived_ = false;
}

bool UWebSocketsClient::isDiscovering() const {
    return discoveryClient_.isDiscovering();
}


void UWebSocketsClient::stopDiscovery() {
    isDiscovering_ = false;
    discoveryClient_.stopDiscovery();
}

void UWebSocketsClient::connect(StrView url, DiscoveryInfoA di, PeerKey key, int timeout) {
    if (!impl_) {
        impl_ = std::make_unique<UWebSocketsClientImpl>();

        impl_->app = new uWS::App();

        impl_->app->ws<PerSocketData>("/*", {
            .compression = uWS::SHARED_COMPRESSOR,
            .maxPayloadLength = 16 * 1024 * 1024,
            .idleTimeout = 16,
            .maxBackpressure = 1 * 1024 * 1024,
            .closeOnBackpressureLimit = false,
            .resetIdleTimeoutOnSend = false,
            .sendPingsAutomatically = true,

            .upgrade = nullptr,
            .open = [](auto* ws) {
            },
            .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
            },

            //.drain = [](auto* ws) {
            //    /* Check ws->getBufferedAmount() here */
            //},
            //.ping = [](auto* ws, std::string_view) {},
            //.pong = [](auto* ws, std::string_view) {},
            .close = [](auto* ws, int code, std::string_view message) {
                /* You may access ws->getUserData() here, but sending or
                 * doing any kind of I/O with the socket is not valid. */
            },
        });

    }

    impl_->app->connect(

    impl_->app->listen

    active_ = true;

    auto& ws = impl_->webSocket;

    ix::SocketTLSOptions opt;
    opt.caFile = "NONE";
    ws.setTLSOptions(opt);

    ws.setUrl(String{url});

    // Optional heart beat, sent every 45 seconds when there is not any traffic
    // to make sure that load balancers do not kill an idle connection.
    //ws.setPingInterval(45);
    ws.disablePong();
    ws.disableAutomaticReconnection();

    //
    // impl_->setDiscoveryInfo(di);
    impl_->di = di;

    // Per message deflate connection is enabled by default. You can tweak its parameters or disable
    // it
    //ws.disablePerMessageDeflate();

    ws.setOnMessageCallback([this](const auto& msg) {
        processRecvMessage(msg.get());
    });

    hostPeerInfo_.key = currentHostPeerKey_++;
    auto status = ws.connect(timeout);

    if (!status.success) {
        events_.pushError(hostPeerInfo_.key, 0, status.errorStr);
        ws.stop();
        return;
    }

    // Send the discovery info
    ws.send(impl_->di.build());

    ws.start();
}

void UWebSocketsClient::processRecvMessage(const void* pmsg) {
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
            connected_ = true;

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



void UWebSocketsClient::disconnect() {
    if (!impl_)
        return;

    impl_->webSocket.close();
    connected_ = false;

    hostPeerInfo_.key = 0;
}

void UWebSocketsClient::shutdown() {
    disconnect();
}

void UWebSocketsClient::send(DataParamA data, bool unreliable) {
    if (!connected_) {
        MDebugTrap();
        return;
    }

    //impl_->app->

    impl_->webSocket.send({(const char *)data.data(), data.size()}, true);
}

void UWebSocketsClient::onTimerPostfix(Timestamp timestamp) {
    //if (!impl_)
    //    return;
    if (isDiscovering_) {
        impl_->discoveryClient.onTimer(timestamp);
    }
}

} // namespace mangl::net


#endif
