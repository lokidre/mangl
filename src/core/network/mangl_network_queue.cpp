/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_network.h"

namespace mangl::net {

void NetworkBaseCommon::processNetworkEvents(Timestamp) {

    if (events_.empty())
        return;

    Event ev{};

    while (events_.pop(ev)) {

        switch (ev.type) {
            using enum Event::Type;
        case Packet:
            if (onData) onData(ev.peerKey, ev.packet.data());
            break;

        case Found:
            if (onPeerFound) {
                DiscoveryInfo di;
                di.parseData(ev.packet.buffer);

                if (di.alias.empty())
                    break;

                //MDebugFunc(di.build());

                onPeerFound(ev.peerKey, di);
            }
            break;
        case State:
            if (onPeerState) onPeerState(ev.peerKey, ev.peerState);
            break;

        case Error:
            if (onError) onError(ev.peerKey, ErrorInfo{ev.packet.code, ev.packet.strView()});
            break;

        default:
            MDebugTrap();
            break;
        }

        //events_.pop();
    }
}

}  // namespace mangl::net
