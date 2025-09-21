/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_mp_client.h"

namespace mangl::mmp {

void GameClientBase::onBaseTimer(Timestamp timestamp) {
    processSystemTimerBody(timestamp);

    if (onTimer) onTimer(timestamp);


    // In the end process all the queued packets
    // TODO: move to a separate thread probable
    while (!packetQueue_.empty()) {
        auto& item = packetQueue_.top();
        transmitData(item.data);
        packetQueue_.pop();
    }
}

void GameClientBase::processSystemTimerBody(Timestamp timestamp) {

    // Process data transfer first (new cycle after all the network updates)

    if (!status_.connected) {
        if (status_.pendingConnect) {
            status_.pendingConnect = false; 
            networkConnect();
        }
        return;
    }

    if (!status_.welcomeReceived) {
        if (!status_.helloSent) {
            status_.helloSent = true;

            auto packet = packetCommandSystem(MsgOrd::Hello);
            packetSend(packet);
        }

        return;
    }

    // Now try to find / join the match
    if (!status_.matchFound) {
        if (!status_.matchFindRequested) {
            status_.matchFindRequested = true;

            auto packet = packetCommandMatch(MsgOrd::MatchFind);

            packetSend(packet);
        }
        return;
    }

    if (!status_.matchJoined) {
        //if (!status_.matchJoinRequested) {
        //    status_.matchJoinRequested = true;

        //    auto packet = packetCommandMatch(MsgOrd::MatchJoin);
        //    packet->matchKey = status_.matchKey;
        //    packet->matchId = status_.matchId;

        //    packetSend(packet);
        //}

        return;
    }

}

}  // namespace

