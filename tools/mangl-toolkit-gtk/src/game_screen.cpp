/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "game_screen.h"

#include "state.hpp"
#include "pubsub.hpp"

namespace toolkit {

void GameScreen::onStart() {

    auto pubsub = Pubsub::instance();

    pubsub->viewOptionsChanged = [this] {onViewOptionsChanged(); };


    buildScene();

//    auto scene = GameScreenApp::instance()->scene();
//
//    auto prim = scene->newRect(0xff0000, {10, 10, 100, 10});

}

void GameScreen::onRelease() {
    auto pubsub = Pubsub::instance();
    pubsub->viewOptionsChanged = nullptr;
    GameScreenApp::getScene()->release();
}

void GameScreen::buildScene() {
    auto state = State::instance();
    auto& projectFiles = state->projectFiles;

    if (state->showBackgroundIndex < 0 || projectFiles.selectedCat != ProjectCategory::Background) {
        buildEmptyBackground();
    }


    switch (projectFiles.selectedCat) {
    case ProjectCategory::Animation:
        buildAnimation();
        break;
    case ProjectCategory::Background:
        break;
    case ProjectCategory::Model:
        buildModel();
        break;
    default:
        break;
    }

}

void GameScreen::buildEmptyBackground() {
    //using namespace mangl;

    auto scene = getScene();
    auto state = State::instance();

    //scene->newRect(0xcd853f, {0, 0, Env::applicationRect.w, 10});
    scene->newBox(0xcd853f, Env::applicationRect);

    auto chsize = state->checkboardSize;


    int rows = (int)(state->screenSize.h / chsize) + 1;
    int cols = (int)(state->screenSize.w / chsize) + 1;

    for (int j = 0; j < rows; ++j) {
        for (int i = 0; i < cols; ++i) {
            if ((i & 1) == (j & 1)) {
                scene->newBox(0xe0e0e0, {i * chsize, j * chsize, chsize, chsize});
            }
        }
    }
}


void GameScreen::onViewOptionsChanged() {
    auto state = State::instance();

    auto& model = *model_;
    model.wireframe = state->model.wireframe;
    model.vertices = state->model.vertices;
    model.faceCulling = state->model.faceCulling ;
    model.depthTest = state->model.depthTest ;
    model.lightingEnabled = state->model.lighting;
}

}  // namespace toolkit
