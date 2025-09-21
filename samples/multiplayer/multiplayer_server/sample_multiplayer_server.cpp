/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../shared/sample_game_backend.hpp"
#include "../shared/sample_game_instance.hpp"

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


namespace sample::mp {

struct App: public AppBase<App> {};

static Main<App> manglMain_;

static bool optDaemonize{};
static int optPort = 13781;

static std::string optScheme = "ws";
static std::string optAddress = "0.0.0.0";
static std::string optDebugPacketLogDir; // = "/tmp/packets";
static std::atomic_bool finishFlag_;

SampleGameServer* gameServer_{};


static void init() {
    ADebugPrintFunc();

    mangl::mangl_main();
    App::instance();
}

static void start() 
{
    ADebugPrintFunc();

    finishFlag_ = false;

    gameServer_ = SampleGameServer::instance();
    
    gameServer_->onDebugMessage = [](auto text) { apl::println(text); };

    gameServer_->onGameInstanceCreate = [] {
        return new SampleGameInstance();
    };

    gameServer_->onGameInstanceDestroy = [](auto instance) {
        delete instance;
    };

    gameServer_->setup({
        .connectionType = net::ConnectionType::Nearby,
        .hostUrl = apl::fmt("%s://%s:%d", optScheme, optAddress, optPort),
        .debugVerboseAll = true,
        //.debugVerboseConData = true,
        .debugPacketLogDir = optDebugPacketLogDir,
    });
    
    gameServer_->start();
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
Sample Multiplayer Game Server
All Rights Reserved

Usage: mangl_sample_multiplayer_server <options>...

Where options:
)";

static int processCmdline(int argc, char* argv[]) {
    apl::CommandLine cmdline;

    cmdline.addFlag({.name = "help", .salt = 'h', .dup = "?",
         .action = [&] { cmdline.printout(helpHeader).stop(); },
         .desc = "Print this message"});

    cmdline.addFlag({.name = "daemonize", .salt = 'd', .bind = &optDaemonize, .desc = "Stay resident."});
    cmdline.add({.name = "port", .salt = 'p', .ibind = &optPort, .desc = "Port to listen to.", .arg = "port"});
    cmdline.add({.name = "address", .salt = 'a', .sbind = &optAddress, .desc = "Address to bind.", .arg = "addr"});

    cmdline.process(argc, argv);

    if (cmdline.stopped())
        return 2;


    return 0;
}

} // namespace server

extern "C" int daemon_exit_handler() { return 0; }

int main(int argc, char* argv[]) {
    int retcode = 1;

    apl::println("Sample standalone server...");

    try {
        if (auto ret = sample::mp::processCmdline(argc, argv); ret != 0)
            return ret;

//#if APEAL_OS_UNIX
//        if (multiplayer::optDaemonize)
//            if (daemonize("pentix_server"))
//                return 0;
//#endif

        retcode = sample::mp::run();

    } catch (std::exception const& e) {
        std::cerr << e.what() << '\n';
    } catch (...) {
        std::cerr << "exception!" << '\n';
    }

    return retcode;
}
