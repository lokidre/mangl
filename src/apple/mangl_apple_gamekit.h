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
#include "../core/network/mangl_network_matchmaker_base.h"

#if APEAL_OS_MAC

// Apple Game center

namespace mangl::net {


class GameKitMatchmaker: public NetworkMatchmakerBase {
protected:
    void doStartMatchmakingGui(mmp::MpGameParams params) override;
    void doStopMatchmakingGui() override;
};


class GameKitClient: public NetworkBaseClient {
public:
    PeerKey serverPeerKey{};
    
protected:
    void doSend(NetworkDataSendParamsA params) override;

    void doConnect() override;
    void doDisconnect() override;
    void doShutdown() override;
};


class GameKitServer: public NetworkBaseServer {
protected:
    void doStartAdvertising() override;
    void doSend(NetworkDataSendParamsA params) override;
    void doDisconnect(PeerKey pk) override;
    void doShutdown() override;
};

}  //  namespae mangl::net

#endif



