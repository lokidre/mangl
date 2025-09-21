/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"

#include "mangl_network_event_queue.h"
#include "mangl_network_discovery_info.h"

#include "mangl_network_peer.h"

namespace mangl::net {

struct NewtworkClientConnectParams {
    String url;
    DiscoveryInfo di;
    PeerKey pk;
    int timeout;
};
using NewtworkClientConnectParamsA = const NewtworkClientConnectParams&;


struct NewtworkServerStartParams {

    // Server Url
    StrView url;
    String surl;
    String scheme;  // defaults to "ws"
    String host;  // defaults to "0.0.0.0"
    unsigned int port;  // defaults to MANGL_NETWORK_DEFAULT_PORT 13781

    // SSL configuration
    bool ssl;

    // Data transmittion options
    bool compress{};
    bool noCompress{};


    DiscoveryInfo di;
};
using NewtworkServerStartParamsA = const NewtworkServerStartParams&;


struct NetworkDataSendParams {
    PeerKey pk{};
    DataParam data;
    bool unreliable{};
};

using NetworkDataSendParamsA = const NetworkDataSendParams&;


class NetworkBaseCommon {
public:
    using OnPeerShouldAsk = std::function<void (bool*, PeerKey, StrView)>;
    using OnError = std::function<void(PeerKey, ErrorInfoA)>;
    using OnPeerFound = std::function<void (PeerKey, DiscoveryInfoA)>;
    using OnPeerState = std::function<void (PeerKey, PeerState)>;
    using OnData = std::function<void (PeerKey, DataParamA)>;

    OnError onError;
    OnPeerShouldAsk onPeerShouldAsk;
    OnPeerFound onPeerFound;
    OnPeerState onPeerState;
    OnData onData;

    void clearCallbacks() noexcept {
        onError = {};
        onPeerShouldAsk = {};
        onPeerFound = nullptr;
        onPeerState = nullptr;
        onData = nullptr;
    }



    NetworkBaseCommon() = default;

    virtual ~NetworkBaseCommon() noexcept {
        shutdown();
    }



    auto& getEventsQueue() noexcept { return events_; }

protected:
    EventQueue events_;
    TimerHandler eventsHandler_;

    void registerEventsHandler() {
        if (eventsHandler_.isRegistered()) return;
        eventsHandler_.registerHandler([this](auto timestamp) { 
            processNetworkEvents(timestamp); 
        });
    }
    void unregisterEventsHandler() { eventsHandler_.unregister(); }
    void processNetworkEvents(Timestamp timestamp);


public:
    static PeerKey generatePeerKey() noexcept;


public:
    void send(NetworkDataSendParamsA params) { doSend(params); }

    void send(PeerKey pk, DataParamA data, bool unreliable = false) {
        doSend({
            .pk = pk,
            .data = data,
            .unreliable = unreliable,
        });
    }
    void sendToAll(DataParamA data, bool unreliable = false) {
        doSend({
            .data = data,
            .unreliable = unreliable,
        });
    }

    void send(DataParamA data, bool unreliable = false) {
        doSend({
            .data = data,
            .unreliable = unreliable,
        });
    }


    void shutdown() noexcept {
        unregisterEventsHandler();
        doShutdownCommon();
        clearCallbacks();
        events_.clear();
    }


protected:
    virtual void doSend(NetworkDataSendParamsA params) {}
    virtual void doShutdownCommon() {}
};


class NetworkBaseClient: public NetworkBaseCommon {
protected:
    bool isDiscovering_{};
    bool discoveryInfoReceived_{};
    bool discovered_{};

    bool connectPending_{};
    bool connected_{};

    Peer hostPeerInfo_{};

    NewtworkClientConnectParams connectParams_{};


public:
    void setHostPeerKey(PeerKey pk) {
        hostPeerInfo_.key = pk;
    }


    void startDiscovery(DiscoveryInfoA di) {
        registerEventsHandler();

        isDiscovering_ = true;
        discoveryInfoReceived_ = false;
        doStartDiscovery(di);
    }
    bool isDiscovering() { return doIsDiscovering(); }
    void stopDiscovery() {
        doStopDiscovery();
        isDiscovering_ = false;

        if (!connectPending_)
            unregisterEventsHandler();
    }

    void connect(NewtworkClientConnectParamsA params) {
        connectParams_ = params;
        if (!connectParams_.timeout)
            connectParams_.timeout = 30;  // 15 seconds connect timeout

        connectPending_ = true;
        registerEventsHandler();

        doConnect();
    }

    void connect(StrView url, DiscoveryInfoA di, PeerKey pk = 0, int timeout = 30) {
        String surl{url};
        connect({
            .url = surl,
            .di = di,
            .pk = pk,
            .timeout = timeout,
        });
    }

    void disconnect() {
        doDisconnect();
        connected_ = false;
    }

    void close() { disconnect(); }
    

protected:
    void doShutdownCommon() override {
        discoveryInfoReceived_ = false;

        stopDiscovery();
        doDisconnect();
        doShutdown();
    }

    void processConnected() {
        connected_ = true;
        connectPending_ = false;
    }


protected:

    virtual void doStartDiscovery(DiscoveryInfoA) {}
    virtual bool doIsDiscovering() { return isDiscovering_; }
    virtual void doStopDiscovery() { isDiscovering_ = false; }

    virtual void doConnect(){}
    virtual void doDisconnect(){}

    virtual void doShutdown() {}
};


class NetworkBaseServer: public NetworkBaseCommon {

protected:
    bool isAdvertising_{};

    NewtworkServerStartParams startParams_{};


public:
    virtual ~NetworkBaseServer() {
        int a = 0;
    }

    void start(NewtworkServerStartParamsA params);

    void start(StrView url, DiscoveryInfoA di) {
        start({
            .url = url,
            .di = di,
        });
    }

    void startListening() { doStartListening(); }
    void stopListening() { doStopListening(); }


    void startAdvertising() { 
        registerEventsHandler();
        doStartAdvertising();
        isAdvertising_ = true;
    }
    bool isAdvertising() { return doIsAdvertising(); }
    void stopAdvertising() {
        doStopAdvertising();
        isAdvertising_ = false;
    }


    void disconnect(PeerKey pk) { doDisconnect(pk); }



protected:
    void doShutdownCommon() override {
        doShutdown();
    }

protected:
    virtual void doStart() {}

    virtual void doStartListening() {}
    virtual void doStopListening() {}

    virtual void doStartAdvertising(){}
    virtual bool doIsAdvertising() { return isAdvertising_; }
    virtual void doStopAdvertising() {}

    virtual void doDisconnect(PeerKey) {}
    virtual void doShutdown() {}
   
};



}  // namespace mangl::net
