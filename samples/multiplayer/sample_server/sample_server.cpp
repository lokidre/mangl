/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
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

#include "daemonize.h"


namespace sample::mp {

using apl::Console;

class App: public AppBase<App> {};

static Main<App> manglMain_;


static bool optDaemonize{};
static int optPort = 13781;
static std::string optAddress = "0.0.0.0";
static std::string hostUrl_;
static std::atomic_bool finishFlag_;

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

    hostUrl_ = apl::fmt("ws://%s:%d", optAddress, optPort);

    mangl::mangl_main();
    App::instance();
}

static void start() {
    ADebugPrintFunc();

    finishFlag_ = false;

    auto server = new net::NearbyServer();

    server->onError = [](PeerKey pk, ErrorInfo err) {
        Console::log("Error! Peer: %d, Code: %d, Message: %s", pk, err.code, err.text);
    };

    server->onPeerFound = [](PeerKey pk, net::DiscoveryInfoA di) {
        Console::log("Peer Found! Peer: %d, Discovery Info: %s", pk, di.build());

        peers_.insert({pk, {pk, di}});
    };

    server->onPeerState = [](PeerKey pk, PeerState st) {
        Console::log("Peer State! Peer: %d, State: %c", pk, M::getPeerStateString(st));

        auto peerIt = peers_.find(pk);

        if (peerIt != peers_.end()) {
            auto& peer = peerIt->second;
            peer.state = st;

            if (PeerState::Disconnected == st) {
                peers_.erase(peerIt);
            }
        }
    };

    server->onData = [server](PeerKey pk, DataParamA data) {
        Console::log("Date Received! Peer: %d, Size: %d", pk, data.size());

        // Relaying data
        for (auto& peerIt: peers_) {
            auto& peer = peerIt.second;
            if (peer.peerKey == pk)
                continue;

            server->send(peer.peerKey, data);
        }
    };


    net::DiscoveryInfo di;
    di.alias = Env::deviceUserName();
    di.proto = ProtocolName;
    di.protv = ProtocolVersion;
    di.appn = "mp_sample_server";
    di.appv = "0.0.1";


    server->start(hostUrl_, di);

    server->startAdvertising();

    Console::log("Listening on: %s ...", hostUrl_);
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
    double timestamp = 0;

    for (;;) {
        if (finishFlag_)
            break;

        auto tickStart = Clock::now();

        timestamp = Duration{tickStart - startTime}.count();

        app->processTimer(timestamp);

        // sleep_for(Duration{0.3});  // Debug

        auto tickEnd = Clock::now();

        auto tickDuration = Duration{tickEnd - tickStart}.count();

        if (tickDuration >= secsPerTick) {
            // We are choking. Show some message or something
            continue;
        }

        auto timeToSleep = secsPerTick - tickDuration;

        sleep_for(Duration{timeToSleep});

        // apl::println("%0.4f", timestamp);  // Debug
    }


    return 0;
}

constexpr static auto helpHeader = R"(
Sample Standalone Multiplayer Game Server
All Rights Reserved

Usage: mp_sample_server <options>...

Where options:
)";


static int processCmdline(int argc, char* argv[]) {
    apl::CommandLine cmdline;

    cmdline.addFlag({.name = "help", .salt = 'h', .dup = "?",
        .action = [&cmdline] { cmdline.printout(helpHeader).stop(); },
        .desc = "Print this message",
    });

    cmdline.addFlag({.name = "daemonize", .salt = 'd', .bind = &optDaemonize, .desc = "Stay resident.",});
    cmdline.add({.name = "port", .salt = 'p', .ibind = &optPort, .desc = "Port to listen to.", .arg = "port", });
    cmdline.add({.name = "address",  .salt = 'a', .sbind = &optAddress,  .desc = "Address to bind.",  .arg = "addr",});


    cmdline.process(argc, argv);

    if (cmdline.stopped())
        return 2;

    return 0;
}

} // namespace server

using namespace sample::mp;

extern "C" int daemon_exit_handler() { return 0; }

int main(int argc, char* argv[]) {
    int retcode = 1;

    Console::log("Sample standalone server...");

    try {
        if (auto ret = processCmdline(argc, argv); ret != 0)
            return ret;

#if APEAL_OS_UNIX
        if (optDaemonize)
            if (daemonize("mp_sample_server"))
                return 0;
#endif

        retcode = run();

    } catch (std::exception const& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "exception!" << '\n';
    }

    return retcode;
}
