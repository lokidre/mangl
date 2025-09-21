/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_types.h"


namespace mangl::mmp {

class MpGameBase;
using MpGameBaseP = MpGameBase*;

struct MpPeer: public net::Peer {
    bool isNpc{};

    Timestamp connectTimestamp{};
    Timestamp lastDataTimestamp{};

    bool isError{};
    int errorCode{};
    String errorString;
    void setError(int code, StrView text) {
        errorCode = code;
        errorString = String{text};
    }

    net::DataTransferFormat format{};

    MpGameBaseP local{};  // Local connection
    bool localConnectPending{};

    
    MpPlayerKey playerKey{};
    MpPlayerP player{};
    String playerId;
    String playerAlias;
    String playerName;

    Timestamp lastConTimestamp{};
};

}  // namespace mangl::mmp

