/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_network_discovery_client.h"
#include "../mangl_framework.h"


#include <apeal/socket.hpp>

#if TARGET_OS_POSIX
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#endif

#if TARGET_OS_WINDOWS
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#endif


namespace mangl::net {

void NetworkDiscoveryClient::startDiscovery(NetworkDiscoveryParamsA params) {
    params_ = params;

    auto& socket = socket_;

    //socket.create(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    socket.create(AF_INET, SOCK_DGRAM, 0);

    socket.timeout_disable();
    socket.bind(discoveryPort_);


    struct ip_mreq mreq{};

//#if TARGET_OS_WINDOWS
    auto res = socket.setOpt(IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
//#else
//
////    mreq.imr_multiaddr.s_addr = inet_addr("192.168.5.255");
////    mreq.imr_interface.s_addr = inet_addr("192.168.5.17");
//
//    res = setsockopt(socket.handle(), IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
//#endif

    isDiscovering_ = true;

    timerHandler_.registerHandler(std::bind(&Self::onTimer, this, std::placeholders::_1));
}

void NetworkDiscoveryClient::stopDiscovery() { 
    timerHandler_.unregister();
    isDiscovering_ = false; 
    socket_.close();
}


void NetworkDiscoveryClient::onTimer(Timestamp timestamp) {
    if (!isDiscovering_)
        return;

    std::size_t count = socket_.ready();

    if (count > 0) {
        int bufferSize = 500;
        char buffer[500+1];

        count = socket_.recvSome(buffer, bufferSize);

        DiscoveryInfo di;
        if (di.parse({buffer, count})) {
            stopDiscovery();
            discovered_ = true;

            params_.cb(di);
        }
    }
}


}  // namespace mangl::net
