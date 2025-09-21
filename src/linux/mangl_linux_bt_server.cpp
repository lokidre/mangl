/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_linux_bt_server.h"
#include "mangl_linux_bt_context.h"

#include <algorithm>
#include <thread>

#if MANGL_BLUETOOTH_DBUS

namespace mangl::net {


namespace impl {


}  // namespace impl


BluetoothServerDbus::BluetoothServerDbus(){}
BluetoothServerDbus::~BluetoothServerDbus(){}




void BluetoothServerDbus::doStart() {
    if (!ctx_)
        ctx_ = std::make_unique<impl::BtDbusContext>();

    auto& ctx = *ctx_;

    ctx.createBusConnection();

    auto proxy = ctx.bluezProxy.get();

    proxy->uponSignal("InterfacesAdded").onInterface(impl::InterfaceDbusObjectManager)
        .call([this](sdbus::ObjectPath path,  const impl::BtDictionary& dictionary) {
            MDebugPrint("BT InterfaceAdded: %s", path);
        });

    proxy->finishRegistration();

    auto powered = ctx.getBoolProperty("Powered");

    if (!powered) {
        ctx.setProperty("Powered", true);
        powered = ctx.getBoolProperty("Powered");
    }

    ctx.setProperty("Discoverable", true);


    //ctx.bluezProxy->setProperty("Powered").onInterface(InterfaceAdapter).toValue(true);

    std::string name = ctx.getProperty("Name");
    std::string address = ctx.getProperty("Address");
    std::string addressType = ctx.getProperty("AddressType");
    auto discoverable = ctx.getBoolProperty("Discoverable");
    ctx.setProperty("Alias", String{"ManglBluez"});

    MDebugPrint("Name: %s", name);
    MDebugPrint("Address: %s; Type: %s", address, addressType);
    MDebugPrint("Powered: %b", powered);
    MDebugPrint("Discoverable: %b", discoverable);

    //ctx.bluezProxy->callMethod("StartDiscovery").onInterface(InterfaceAdapter);

    //auto btstatus = getBluetoothStatus();

    // Start advertising
    ctx.bluezGattProxy->callMethod("RegisterApplication")
        .onInterface(impl::InterfaceGattManager)
        .withArguments(ctx.appPath);

    //.([this](sdbus::ObjectPath path,)


    int a =9;
}

void BluetoothServerDbus::doStartAdvertising() {

    auto& ctx = *ctx_;

    ctx.bluezProxy->callMethod("RegisterAdvertisement")
        .onInterface(impl::InterfaceAdvertisingManager)
        .withArguments(ctx.advPath);

    isAdvertising_ = true;
}

void BluetoothServerDbus::doStopAdvertising() {
    isAdvertising_ = false;
}


void BluetoothServerDbus::doShutdown() {
    if (!ctx_)
        return;

    auto& ctx = *ctx_;



    ctx_.reset();
    ctx_ = nullptr;
}




}  // namespace mangl::net

#endif // If Bluetoot DBus is enabled

