/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_network_base.h"

#include <apeal/parse/url.hpp>

#include <atomic>


namespace mangl::net {


static std::atomic_int currentPeerKey_{1};


PeerKey NetworkBaseCommon::generatePeerKey() noexcept {
    
//    auto pk = M::generateId<PeerKey>();

    auto pk = currentPeerKey_++;

    return pk;
}


void NetworkBaseServer::start(NewtworkServerStartParamsA params) {
    startParams_ = params;

    auto& sp = startParams_;

    if (!params.url.empty())
        sp.surl = String{params.url};

    if (startParams_.surl.empty()) {
        auto u = apl::parseUrl(sp.surl);

        if (!u.scheme.empty()) startParams_.scheme = u.scheme;
        if (!u.host.empty()) startParams_.host = u.host;
        if (u.port) startParams_.port = u.port;
    }

    if (startParams_.scheme.empty())
        startParams_.scheme = "ws";
    if (startParams_.host.empty())
        startParams_.host = "0.0.0.0";

    //if (!startParams_.port && !startParams_.port)
    //    startParams_.port = std::to_string(startParams_.iport);

    if (!startParams_.port) {
#ifdef MANGL_NETWORK_DEFAULT_PORT
        startParams_.port = MANGL_NETWORK_DEFAULT_PORT;
#else
        startParams_.port = 13781;
#endif
    }

    sp.compress = true;

    if (params.noCompress)
        sp.compress = false;

    registerEventsHandler();

    doStart();
}




}  // namespace mangl::net

