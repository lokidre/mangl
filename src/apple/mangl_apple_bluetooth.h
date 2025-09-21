/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/network/mangl_network_base.h"

namespace mangl::net {


class BluetoothServerApple: public NetworkBaseServer {
public:
    static bool isBluetoothAvailable() {return true;}
    
protected:
    void doStart() override;

};

class BluetoothClientApple: public NetworkBaseServer {
public:
    static bool isBluetoothAvailable() {return true;}
};



}  // namespace mangl



