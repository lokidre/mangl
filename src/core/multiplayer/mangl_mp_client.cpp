/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_client.h"

#include "../mangl_network.h"


namespace mangl::mmp {

GameClientBase::~GameClientBase() {
    shutdownGameClient();
}

void GameClientBase::shutdownGameClient() {
    MpGameBase::shutdownMpGameBase();

    if (network_) {
        network_->clearCallbacks();
        network_->shutdown();
        delete network_;
        network_ = nullptr;
    }
}



void GameClientBase::setupGameClient(const MpGameParams& params) {
    MpGameBase::setupMpGameBase(params);
}


void GameClientBase::startGameClient() {
    MpGameBase::startMpGameBase();

    setupNetwork();


    // Todo check if discovery needed
    // Start discovery
    if (params_.hostDiscovery) {
        debugConStatus("Discovering", 0, 0, clientPeer_.di.build());
        network_->startDiscovery(clientPeer_.di);
    }

    if (params_.localServer && !status_.isNpc) {
        params_.localServer->localClientConnect(this, clientPeer_.di);
    }
}



}  // namespace mmp

