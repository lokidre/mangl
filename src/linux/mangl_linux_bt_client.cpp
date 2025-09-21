/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_linux_bt_client.h"
#include "mangl_linux_bt_context.h"

#if MANGL_BLUETOOTH_DBUS

namespace mangl::net {

BluetoothClientDbus::BluetoothClientDbus(){}
BluetoothClientDbus::~BluetoothClientDbus(){}


void BluetoothClientDbus::doStartDiscovery(DiscoveryInfoA) {
    if (!ctx_)
        ctx_ = std::make_unique<impl::BtDbusContext>();

    auto& ctx = *ctx_;

    ctx.createBusConnection();

    auto proxy = ctx.bluezProxy.get();

    auto powered = ctx.getBoolProperty("Powered");

    if (!powered) {
        ctx.setProperty("Powered", true);
        powered = ctx.getBoolProperty("Powered");
    }

    std::string name = ctx.getProperty("Name");
    std::string address = ctx.getProperty("Address");
    std::string addressType = ctx.getProperty("AddressType");
    auto discoverable = ctx.getBoolProperty("Discoverable");

    MDebugPrint("Name: %s", name);
    MDebugPrint("Address: %s; Type: %s", address, addressType);
    MDebugPrint("Powered: %b", powered);
    MDebugPrint("Discoverable: %b", discoverable);

}

void BluetoothClientDbus::doStopDiscovery() {
    isDiscovering_ = false;
}

void BluetoothClientDbus::doConnect() {
}

void BluetoothClientDbus::doDisconnect() {
}

void BluetoothClientDbus::doShutdown() {
}


}  // namespace mangl::net

#endif // If Bluetoot DBus is enabled


