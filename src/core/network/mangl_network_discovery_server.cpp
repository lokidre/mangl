/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_network_discovery_server.h"
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


void NetworkDiscoveryServer::startAdvertising(NetworkAdvertisingParamsA params) {

    params_ = params;

    auto& socket = socket_;

    String broadcastHost;

#if TARGET_OS_POSIX
    //String iface;
    char hostname[NI_MAXHOST]{};

    ifaddrs* addrs{};
    getifaddrs(&addrs);


    for (struct ifaddrs* ifa = addrs; ifa; ifa = ifa->ifa_next) {
        auto family = ifa->ifa_addr->sa_family;

        if (family != AF_INET)
            continue;

        auto size = (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
        auto s = getnameinfo(
            ifa->ifa_addr,
            static_cast<socklen_t>(size),
            hostname,
            NI_MAXHOST,
            nullptr,
            0,
            NI_NUMERICHOST);

        if (strcmp(ifa->ifa_name, "lo") == 0)
            continue;

        if ((ifa->ifa_flags & IFF_RUNNING) == 0)
            continue;

        broadcastHost = hostname;

        //iface = ifa->ifa_name;

        //s = getnameinfo(ifa->ifa_name, size, hostname, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);
//        s = getnameinfo(ifa->ifa_netmask, size, hostname, NI_MAXHOST, nullptr, 0, NI_NUMERICHOST);

       s = getnameinfo(
           ifa->ifa_dstaddr,
           static_cast<socklen_t>(size),
           hostname,
           NI_MAXHOST,
           nullptr,
           0,
           NI_NUMERICHOST);

       broadcastAddress_ = hostname;
       break;
    }
#else
    DWORD aaSize{};


    GetAdaptersInfo(0, &aaSize);

    apl::verify(aaSize > 0, "No network adapters found");


    auto adaptersInfoBuffer = std::make_unique<uint8_t[]>(aaSize);
    auto adaptersInfo = (IP_ADAPTER_INFO *)(adaptersInfoBuffer.get());

    GetAdaptersInfo(adaptersInfo, &aaSize);

    for (auto pai = adaptersInfo; pai ; pai = pai->Next) {
        if (pai->Type != MIB_IF_TYPE_ETHERNET)
            continue;

        if (strcmp(pai->IpAddressList.IpAddress.String, "0.0.0.0") == 0)
            continue;

        broadcastHost = pai->IpAddressList.IpAddress.String;
        broadcastAddress_ = pai->IpAddressList.IpAddress.String;
        //broadcastAddress_ = "255.255.255.255";
        //broadcastAddress_ = "255.255.255.0"; // pai->GatewayList.IpAddress.String;
    }


    //
    // Another way of determining the local addresses
    //
    //GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, 0, &aaSize);
    //apl::verify(aaSize > 0, "No network interface found");
    //auto addresses = std::make_unique<IP_ADAPTER_ADDRESSES[]>(aaSize);
    //GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, addresses.get(), &aaSize);
    //int bufSize = 1024;
    //char buf[1024];
    //for (auto pAddr = addresses.get(); pAddr; pAddr = pAddr->Next) {
    //    for (auto iaddr = pAddr->FirstUnicastAddress; iaddr; iaddr = iaddr->Next) {
    //        if ((iaddr->Flags & IP_ADAPTER_ADDRESS_DNS_ELIGIBLE) != 0) {
    //            memset(buf, 0, bufSize);
    //            getnameinfo(iaddr->Address.lpSockaddr, iaddr->Address.iSockaddrLength, buf, bufSize, NULL, 0, NI_NUMERICHOST);
    //            broadcastHost = buf;
    //        }
    //    }
    //}



#endif

    params_.di.setScheme(params.scheme);
    params_.di.setHost(broadcastHost);
    params_.di.setPort(params.port);

    advertisingData_ = params_.di.build();

    // Create UDP socket
    socket.create(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Enable broadcast flag
    int enabled = 1;
    socket.setOpt(SOL_SOCKET, SO_BROADCAST, enabled);

    //auto res = setsockopt(socket.handle(), SOL_SOCKET, SO_BROADCAST, (char *)&enabled, sizeof(enabled));
    //auto res = setsockopt(socket.handle(), SOL_SOCKET, SO_BINDTODEVICE, iface.c_str(), iface.size());



    memset(&servaddr_, 0, sizeof(servaddr_));

    servaddr_.sin_family = AF_INET;
    servaddr_.sin_port = htons(static_cast<u_short>(discoveryPort_));

#if TARGET_OS_WINDOWS
    inet_pton(AF_INET, broadcastAddress_.c_str(), &servaddr_.sin_addr);
#else
    inet_aton(broadcastAddress_.c_str(), &servaddr_.sin_addr);
#endif



    advertizeTimestamp_ = Framework::currentTimestamp();
    isAdvertizing_ = true;

    timerHandler_.registerHandler(std::bind(&Self::onTimer, this, std::placeholders::_1));
}

void NetworkDiscoveryServer::stopAdvertising() { 
    timerHandler_.unregister();

    isAdvertizing_ = false; 
    socket_.close();
}



void NetworkDiscoveryServer::onTimer(Timestamp timestamp) {
    if (!isAdvertizing_)
        return;

    if (timestamp < advertizeTimestamp_)
        return;

    auto data = advertisingData_.data();
    auto size = static_cast<int>(advertisingData_.size());
    auto addr = reinterpret_cast<const struct sockaddr*>(&servaddr_);
    int flags = 0;

#if TARGET_OS_WINDOWS
    flags = MSG_DONTROUTE;
#endif

    //::sendto(hsocket, data, size, MSG_DONTWAIT, addr, sizeof(servaddr));
    //auto res = ::sendto(hsocket, data, size, 0, addr, sizeof(servaddr));
    auto res = socket_.sendTo(data, size, flags, addr, sizeof(servaddr_));

    advertizeTimestamp_ = timestamp + advertizingPeriod_;
}

}  // namespace mangl::net
