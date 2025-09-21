/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_setup.h"


#if MANGL_BLUETOOTH_JNI

#include "../core/network/mangl_network_base.h"

namespace mangl::net {

class BluetoothServerJni: public NetworkBaseServer {
    using Base = NetworkBaseServer;

public:
    static bool isBluetoothAvailable();

protected:
    void doStart() override;
    void doStartAdvertising() override;
    void doStopAdvertising() override;

    void doSend(NetworkDataSendParamsA params) override;

    void doDisconnect(PeerKey) override;
    void doShutdown() override;
};



class BluetoothClientJni: public NetworkBaseClient {
    using Base = NetworkBaseClient;
public:
    static bool isBluetoothAvailable();

protected:
    void doStartDiscovery(DiscoveryInfoA) override;
    void doStopDiscovery() override;
    void doSend(NetworkDataSendParamsA params) override;
};


}  // namespace mangl::net


#endif
