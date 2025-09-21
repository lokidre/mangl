/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../shared/sample_game_frontend.hpp"

#include <mangl/multiplayer.h>
#include <mangl/app.h>
#include <mangl/framework.h>
#include <mangl/environment.h>

#include <apeal/cmdline.hpp>
#include <apeal/console.hpp>

#include <stdexcept>
#include <iostream>

#include <chrono>
#include <thread>


using apl::println;

namespace sample::mp {


struct App: public AppBase<App> {};

static Main<App> manglMain_;

static bool optDaemonize{};
static int optPort = 13781;
static std::string optAddress = "0.0.0.0";
//static std::string optDataFormat;
static std::string optDataFormat = "yaml";
//static std::string optDataFormat = "json";
static std::atomic_bool finishFlag_;


SampleGameFrontend* gameClient_{};


static void init() {
    ADebugPrintFunc();

    mangl::mangl_main();
    App::instance();
}

static void start() 
{
    ADebugPrintFunc();

    finishFlag_ = false;

    gameClient_ = SampleGameFrontend::instance();

    gameClient_->onDebugMessage = [](auto text) { apl::println(text); };

    gameClient_->setup({
        .nodeAlias = "console",
        .connectionType = net::ConnectionType::Nearby,
        .dataTransferFormat = net::DiscoveryInfo::parseFormat(optDataFormat, net::DataTransferFormat::Packet),
    });

    gameClient_->onMatch = [](auto packet) {
        auto client = SampleGameFrontend::instance();

        if (packet->isMatchLaunch())  return client->sendMatchReady();
        if (packet->isMatchCountdown()) return println("Countdown: %s", packet->value);
        if (packet->isMatchBegin()) return println("Match Begin!");
        if (packet->isMatchPaused()) return println("Match Paused.");
        if (packet->isMatchResumed()) return println("Match Resumed.");
    };

    gameClient_->onGame = [](auto packet) {
        if (packet->isGameState()) return println("Game State.");
    };



    gameClient_->start();

    //println("Listening on: %s ...", gameServer_.getHostUrl());
}

static void onTimer(Timestamp timestamp) {
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
    Timestamp timestamp{};

    for (;;) {
        if (finishFlag_)
            break;

        auto tickStart = Clock::now();

        timestamp = Duration{tickStart - startTime}.count();

        app->processTimer(timestamp);

        onTimer(timestamp);


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
Sample Multiplayer Game Console Client
All Rights Reserved

Usage: mangl_sample_multiplayer_client <options>...

Where options:
)";

static int processCmdline(int argc, char* argv[]) {
    apl::CommandLine cmdline;

    cmdline.addFlag({.name = "help", .salt = 'h', .dup = "?",
         .action = [&] { cmdline.printout(helpHeader).stop(); },
         .desc = "Print this message"});

    cmdline.add({.name = "port", .salt = 'p', .ibind = &optPort, .desc = "Port to listen to.", .arg = "port"});
    cmdline.add({.name = "address", .salt = 'a', .sbind = &optAddress, .desc = "Address to bind.", .arg = "addr"});

    cmdline.process(argc, argv);

    if (cmdline.stopped())
        return 2;


    return 0;
}

} // namespace 

using namespace sample::mp;

int main(int argc, char* argv[]) {
    int retcode = 1;

    apl::println("Sample multiplayer client...");

    try {
        if (auto ret = processCmdline(argc, argv); ret != 0)
            return ret;

        retcode = run();

    } catch (std::exception const& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "exception!" << '\n';
    }

    return retcode;
}
