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
#include "mangl_network_peer.h"
#include "../multiplayer/mangl_mp_params_game.h"


namespace mangl::net {

class NetworkMatchmakerBase {
public:
    virtual ~NetworkMatchmakerBase() = default;

    using MatchPeerContainer = std::vector<mmp::MpPeer>;
    using OnMatchmakingComplete = std::function<void (const MatchPeerContainer&, const mmp::MpPeer&)>;

    OnMatchmakingComplete onMatchmakingComplete;
    
    void clearCallbacks() noexcept {
        onMatchmakingComplete = {};
    }


    void startMatchmakingGui(mmp::MpGameParams params) {
        doStartMatchmakingGui(params);
    }
    
    void stopMatchmakingGui() {
        doStopMatchmakingGui();
    }
    
    void processMatchmakingComplete(const MatchPeerContainer& peers, const mmp::MpPeer& local) {
        if (onMatchmakingComplete) onMatchmakingComplete(peers, local);
    }
    
protected:
    virtual void doStartMatchmakingGui(mmp::MpGameParams params){}
    virtual void doStopMatchmakingGui(){}

};

}  // namespace mangl::net

