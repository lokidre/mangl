/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "game_screen_app.h"
#include "game_screen.h"

#include "pubsub.hpp"

namespace toolkit {

Main<GameScreenApp> manglMain_;

void GameScreenApp::onInit() {
}

void GameScreenApp::onStart() {
    setupLayout();

    switchScreen(new GameScreen);

    auto pubsub = Pubsub::instance();
    pubsub->projectFileSelected = [this]{ onProjectFileSelected(); };
}

void GameScreenApp::onProjectFileSelected() {
    setupLayout();
    switchScreen(new GameScreen);
}

void GameScreenApp::setupLayout() {
    auto state = State::instance();

    //auto screenSize = Env::applicationRect.size();

    //Env::interfaceIdiom = sampleInterfaceIdiom_;
    Env::applicationRect = {};
    Env::interfaceIdiom = InterfaceIdiom::Phone;
    Env::setApplicationSize(state->screenSize);

    //Env::applicationRect = appRect.convert<Rect>();


    //    if (sampleApplicationLayout_.h == 0)
    //        Env::setLayoutWidth(sampleApplicationLayout_.w);
    //    else
    //        Env::setLayout(sampleApplicationLayout_);

    Env::setLayout(state->screenSize);

}


}  // namespace toolkit
