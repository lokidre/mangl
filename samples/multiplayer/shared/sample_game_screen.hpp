/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl_test.h>

#ifndef MANGL_CONSOLE

#include "sample_game_frontend.hpp"

#include <mangl/game.h>

namespace sample::mp {

struct SampleGameMode: public apl::Singleton<SampleGameMode> {
    SampleGameFrontend* client{};
};

struct SampleGameElement {
    mmp::PacketKey key{};
    mmp::PacketKey playerKey{};

    PrimP prim{};

    mmp::PacketKey type{};
    Color color{};
    Rect rect{};
};

struct SampleGameFrontendState {
    bool guiCreated{};

    std::unordered_map<mmp::PacketKey, SampleGameElement> elements;
};

class SampleGameScreen: public Screen {
    using Packet = SampleGamePacket;
    using PacketP = SampleGamePacketP;
    using PacketCP = SampleGamePacketCP;

    SampleGameMode* gameMode_ = SampleGameMode::instance();
    SampleGameFrontend* client_ = SampleGameMode::instance()->client;

    PrimP userTap_{};
    bool gameBegan_{};
    bool gamePaused_{};

    SampleGameFrontendState state_;


    void createScene() {
        buildSampleBackground();
        auto statusRect = buildStatusBar();

        auto scene = App::getScene();
        auto safe = Env::getSafeArea();


        userTap_ = scene->newCircle(M::Red, {0, 0}, 20, M::Hidden);
    }

    void initGame() {
        client_->onMatch = [this](auto packet) {
            if (packet->isMatchCountdown()) {
                //gameBegan_ = true;
            }
            if (packet->isMatchBegin())
                gameBegan_ = true;
        };

        client_->onGame = [this](auto packet) {
            if (packet->isGameState()) return updateGameState(packet);
        };

        client_->sendMatchReady();
    }

    void updateGameState(PacketCP packet) {
        if (!state_.guiCreated) {
            createGameStateGui(packet);
            return;
        }

        //for (auto& pel: packet->elements) {
        //    auto it = state_.elements(
        //}

    }

    void createGameStateGui(PacketCP packet) {
        state_.guiCreated = true;

        auto scene = App::getScene();

        for (auto& pel: packet->elements) {
            SampleGameElement el{};
            el.key = pel.key;
            el.playerKey = pel.player;
            el.color = pel.color;
            el.rect = pel.rect;
            el.prim = scene->newBox(el.color, el.rect);
            state_.elements.emplace(el.key, std::move(el));
        }
    }

    //void gameStart() {
    //    auto safe = Env::getSafeArea();

    //    userTap_->setVisible();
    //}

protected:
    void onStart() override {
        createScene();
        initGame();
        //gameStart();
    }

    void onRelease() override {
        gameMode_->client->clearCallbacks();

        if (gameMode_->client) {
            gameMode_->client = gameMode_->client->release();
        }
    }

    void onTimer(Timestamp) override {
        if (!gameBegan_) return;
        //if (client_->isStageLaunched())
        //    return client_->
    }

    
    void onTouchDown(TouchInfoA ti) override {
        if (!gameBegan_) return;
        userTap_->setCircle(ti.p, 20);
    }

};

}  // namespace sample::mp

#endif


