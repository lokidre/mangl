/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_common.h"

#include "network/mangl_network_base.h"
#include "network/mangl_network_matchmaker_base.h"

#include "network/mangl_ixwebsocket_client.h"
#include "network/mangl_ixwebsocket_server.h"
#include "network/mangl_uwebsockets_client.h"
#include "network/mangl_uwebsockets_server.h"

#include "network/mangl_network_discovery_client.h"
#include "network/mangl_network_discovery_server.h"

#if MANGL_BLUETOOTH

#if MANGL_BLUETOOTH_DBUS
#include "../linux/mangl_linux_bt_client.h"
#include "../linux/mangl_linux_bt_server.h"
namespace mangl::net {
using BluetoothServer = BluetoothServerDbus;
using BluetoothClient = BluetoothClientDbus;
}  // namespace mangl::net
#elif MANGL_BLUETOOTH_JNI
#include "../android/mangl_and_jni_bluetooth.h"
namespace mangl::net {
using BluetoothServer = BluetoothServerJni;
using BluetoothClient = BluetoothClientJni;
}  // namespace mangl::net
#elif APEAL_OS_MAC
#include "../apple/mangl_apple_bluetooth.h"
namespace mangl::net {
using BluetoothServer = BluetoothServerApple;
using BluetoothClient = BluetoothClientApple;
}  // namespace mangl::net
#endif

#endif


#if APEAL_OS_MAC
#include "../apple/mangl_apple_gamekit.h"
#include "../apple/mangl_apple_nearby.h"
#endif


namespace mangl::net {


#if MANGL_UWEBSOCKETS
using NearbyServer = UWebSocketsServer;
using NearbyClient = IxWebSocketClient;

using OnlineServer = UWebSocketsServer;
using OnlineClient = IxWebSocketClient;

#elif MANGL_IXWEBSOCKET
using NearbyServer = IxWebSocketServer;
using NearbyClient = IxWebSocketClient;

using OnlineServer = IxWebSocketServer;
using OnlineClient = IxWebSocketClient;

#elif APEAL_OS_MAC

using NearbyServer = AppleNearbyServer;
using NearbyClient = AppleNearbyClient;

using OnlineServer = GameKitServer;
using OnlineClient = GameKitClient;

#endif


#if APEAL_OS_MAC
using OnlineMatchmaker = GameKitMatchmaker;
#else
using OnlineMatchmaker = NetworkMatchmakerBase;
#endif

}  // namespace mangl::net

