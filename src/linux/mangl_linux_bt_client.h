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

#if MANGL_BLUETOOTH_DBUS

#include "../core/network/mangl_network_base.h"
#include "../core/network/mangl_network_discovery_client.h"

namespace mangl::net {

namespace impl {
struct BtDbusContext;
}  // namespace impl

class BluetoothClientDbus: public NetworkBaseClient {
    std::unique_ptr<impl::BtDbusContext> ctx_;
public:
    BluetoothClientDbus();
    ~BluetoothClientDbus();

    static bool isBluetoothAvailable() { return true; }  // TODO: bluetooth available


protected:
    void doStartDiscovery(DiscoveryInfoA) override;
    void doStopDiscovery() override;

    void doConnect() override;
    void doDisconnect() override;

    void doShutdown() override;

};

}  // namespace mangl

#endif

