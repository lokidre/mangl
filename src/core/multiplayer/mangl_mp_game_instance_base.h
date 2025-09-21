/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_server_base.h"

namespace mangl::mmp {



class GameInstanceBase {
    friend class GameServerBase;

public:
    virtual ~GameInstanceBase() = default;

private:
    void processMatchLaunch(MpMatchP match, MpPacketP packet) {
        onServerMatchLaunch(match, packet);
    }

    void processMatchBegin(Timestamp timestamp) {
        onServerMatchBegin(timestamp);
    }


protected:
    virtual void onServerInstance(GameServerBaseP server) = 0;
    virtual void onServerMatchLaunch(MpMatchP match, MpPacketP packet) = 0;
    virtual void onServerMatchBegin(Timestamp timestamp) = 0;
    virtual void onServerTimer(Timestamp timestamp) = 0;
    virtual void onServerActionPacket(MpPacketCP packet) = 0;
    virtual void onServerCommandPacket(MpPlayerCP player, MpPacketCP packet) = 0;
    virtual void onServerMatchEnd() = 0;
};

}  // namespace mangl::mmp

