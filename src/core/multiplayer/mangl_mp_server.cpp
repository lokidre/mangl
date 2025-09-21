/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_server_base.h"

#include "../mangl_network.h"

#include <apeal/uid.hpp>
#include <apeal/uuid.hpp>

namespace mangl::mmp {

MpPlayerKey GameServerBase::currentPlayerIndex_{1};
MpMatchKey GameServerBase::currentMatchIndex_{1};


GameServerBase::~GameServerBase() {
    shutdownGameServer();
}



String GameServerBase::generatePlayerId() {

    return "IP" + apl::generateUid64Base58();
}

String GameServerBase::generateMatchId() {
    return "IM" + apl::generateUid64Base58();
}



void GameServerBase::startGameServer() 
{

    //
    // Connection
    //
    setupNetwork();

    // Start connection
    network_->start(params_.hostUrl, params_.di);

    if (params_.debugVerboseConState) {
        auto text = apl::fmt("Listening on: %s ...", getHostUrl());
        if (onDebugMessage) onDebugMessage(text);
        if (onDebugMessageConnection) onDebugMessageConnection(text);
    }


    if (params_.hostDiscovery) {
        network_->startAdvertising();
    }

    //onConnectionStart(params_.hostUrl, params_.di);
    //onStartAdvertising();

    MpGameBase::startMpGameBase();

    //started_ = true;
}


void GameServerBase::shutdownGameServer() {
    MpGameBase::shutdownMpGameBase();

    if (network_) {
        network_->clearCallbacks();
        network_->shutdown();
        delete network_;
        network_ = nullptr;
    }
}


}  // namespace mmp

