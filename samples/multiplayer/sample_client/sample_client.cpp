/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "sample_client_packet.h"
#include "../shared/sample_mp_protocol.hpp"


#include <mangl/network.h>
#include <mangl/app.h>
#include <mangl/framework.h>
#include <mangl/environment.h>

#include <apeal/cmdline.hpp>
#include <apeal/console.hpp>

#include <stdexcept>
#include <iostream>

#include <chrono>
#include <thread>

using apl::Console;

namespace sample::mp {

class App: public AppBase<App> {};

static Main<App> manglMain_;

static std::string hostUrl_;
static std::atomic_bool finishFlag_;

static bool connected_{};
static net::NearbyClient* connection_{};

static bool packetSent_{};
static Timestamp packetSentTimestamp_{};
static String currentPacket_{};

Timestamp packageTimestamp = 0;
Timestamp timestamp = 0;


static void init() {
    ADebugPrintFunc();

    mangl_main();
    App::instance();
}

static void start() {
    ADebugPrintFunc();

    finishFlag_ = false;

    net::DiscoveryInfo di;
    di.alias = Env::deviceUserName();
    di.proto = ProtocolName;
    di.protv = ProtocolVersion;
    di.appn = "mp_sample_client";
    di.appv = "0.0.1";

    auto connection = new net::NearbyClient();

    connection->onError = [](auto pk, auto err) {
        Console::error("Error: %s", err.text);
        finishFlag_ = true;
    };

    connection->onPeerFound = [connection, di](auto, auto pdi) {
        connection->stopDiscovery();

        Console::log("Discovered: %s", pdi.build());

        hostUrl_ = pdi.getUrl();
        connection->connect(hostUrl_, di);
    };

    connection->onPeerState = [](auto, auto st) {
        Console::log("Connection Status: %c", M::getPeerStateString(st));

        if (PeerState::Connected == st) {
            connected_ = true;
        } else {
            connected_ = false;
        }
    };

    connection->onData = [](auto, auto data) {
        if (checkPacket(currentPacket_, data)) {
            packetSent_ = false;
            Console::log("Packets are matching");
            packageTimestamp = timestamp + 1;
        } else {
            Console::error("Packet Error: Packets don't match");
            finishFlag_ = true;
        }
    };


    connection->startDiscovery(di);

    Console::log("Discovering...");

    connection_ = connection;
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


    while (!finishFlag_) {

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

        if (timestamp >= packageTimestamp) {
            if (connected_) {
                if (packetSent_) {
                    Console::log("Packet receive timeout!");
                    packetSent_ = false;
                }

                if (!packetSent_) {
                    currentPacket_ = buildPacket();
                    Console::log("Sending packet: %d", currentPacket_.size());
                    connection_->send(currentPacket_);
                    packetSent_ = true;
                    packetSentTimestamp_ = timestamp;
                }
            }
            packageTimestamp = timestamp + 1;
        }
    }


    return 0;
}

constexpr static auto helpHeader = R"(
Sample Multiplayer Client
All Rights Reserved

Usage: mp_sample_client <options>...

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

} // namespace multiplayer


using namespace sample::mp;

int main(int argc, char* argv[]) {
    int retcode = 1;

    Console::log("Sample standalone server...");

    try {
        if (auto ret = processCmdline(argc, argv); ret != 0)
            return ret;

        retcode = run();

    } catch (std::exception const& e) {
        Console::error(e.what());
    } catch (...) {
        Console::error("exception!");
    }

    return retcode;
}
