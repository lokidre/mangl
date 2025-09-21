/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "sample_echo_packet.h"

#include "../shared/sample_mp_protocol.hpp"


#include <mangl/network.h>
#include <mangl/framework.h>
#include <mangl/app.h>
#include <mangl/environment.h>

#include <apeal/cmdline.hpp>
#include <apeal/console.hpp>

#include <stdexcept>
#include <iostream>

#include <chrono>
#include <thread>


namespace sample::mp {

using apl::Console;

struct App: public AppBase<App> {};

static Main<App> manglMain_;


static std::string hostUrl_;
static std::atomic_bool finishFlag_;

static bool clientConnected_{};
static net::NearbyClient* client_{};

static bool packetSent_{};
static String currentPacket_{};

struct Peer {
    PeerKey peerKey;
    net::DiscoveryInfo di;
    PeerState state{};

    Peer(PeerKey ikey, net::DiscoveryInfoA idi) {
        peerKey = ikey;
        di = idi;
    }
};

std::unordered_map<PeerKey, Peer> peers_;

static void init() {
    ADebugPrintFunc();

    //hostUrl_ = apl::fmt("ws://%s:%d", optAddress, optPort);

    mangl::mangl_main();
    App::instance();
}

static void start() {
    using namespace mangl;

    ADebugPrintFunc();

    finishFlag_ = false;

    net::DiscoveryInfo di;
    di.alias = Env::deviceUserName();
    di.proto = ProtocolName;
    di.protv = ProtocolVersion;
    di.appn = "mp_echo";
    di.appv = "0.0.1";

    auto connection = new net::NearbyClient();


    connection->startDiscovery(di);

    connection->onError = [](PeerKey pk, ErrorInfo err) {
        Console::error("Error: %s", err.text);
        finishFlag_ = true;
    };

    connection->onPeerFound = [connection, di](PeerKey peerKey, net::DiscoveryInfoA pdi) {
        connection->stopDiscovery();

        Console::log("Discovered: %s", pdi.build());

        hostUrl_ = pdi.getUrl();

        connection->connect(hostUrl_, di);
    };

    connection->onPeerState = [](auto, auto st) {
        Console::log("Connection Status: %s", M::getPeerStateString(st));

        if (PeerState::Connected == st) {
            clientConnected_ = true;
        } else {
            clientConnected_ = false;
        }

    };

    connection->onData = [connection](auto, auto data) {
        Console::log("Echoing packet: %d", data.size());
        connection->send(data);
    };

    Console::log("Discovering...");

    client_ = connection;
}

static int run() {
    ADebugPrintFunc();

    init();

    using Clock = std::chrono::high_resolution_clock;
    using Duration = std::chrono::duration<double>;
    using namespace std::chrono_literals;

    using std::this_thread::sleep_for;

    std::chrono::nanoseconds lag(0ns);

    Timestamp ticksPerSec{30}; // 60; // Ticks per second
    Timestamp secsPerTick{1 / ticksPerSec};

    auto app = App::instance();

    app->onInit();
    app->onStart();
    app->onInitialized();


    start();

    auto startTime = Clock::now();
    Timestamp timestamp = 0;

    while (!finishFlag_) {

        auto tickStart = Clock::now();

        timestamp = Duration{tickStart - startTime}.count();

        app->processTimer(timestamp);

        auto tickEnd = Clock::now();
        auto tickDuration = Duration{tickEnd - tickStart}.count();

        if (tickDuration >= secsPerTick) {
            // We are choking. Show some message or something
            continue;
        }

        auto timeToSleep = secsPerTick - tickDuration;

        sleep_for(Duration{timeToSleep});
    }


    return 0;
}

constexpr static auto helpHeader = R"(
Sample Multiplayer Echo Client
All Rights Reserved

Usage: mp_sample_echo <options>...

Where options:
)";

static int processCmdline(int argc, char* argv[]) 
{
    apl::CommandLine cmdline;


    cmdline.addFlag({.name = "help", .salt = 'h', .dup = "?",
        .action = [&cmdline] { cmdline.printout(helpHeader).stop(); },
        .desc = "Print this message",
    });


    cmdline.process(argc, argv);

    if (cmdline.stopped())
        return 2;


    return 0;
}

} // namespace sample::mp

using namespace sample::mp;


int main(int argc, char* argv[]) {
    int retcode = 1;

    Console::log("Sample multiplayer echo client...");

    try {
        if (auto ret = processCmdline(argc, argv); ret != 0)
            return ret;

        retcode = run();

    } catch (std::exception const& e) {
        Console::error(e.what());
    } catch (...) {
        Console::error("Exception!");
    }

    return retcode;
}
