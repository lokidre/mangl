/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "app.h"

#include "animation_screen.h"
#include "animator_mode.h"
#include "res.h"
#include "stats_display.h"



namespace animator {

static Main<App> manglMain_;


void App::onInit()
{
    animatorScreen = new AnimatorScreen;

    auto mode = AnimatorMode::instance();
    mode->onUpdate();
}


void App::onStart()
{
    switchScreen(animatorScreen);

    for (int tab = 0; tab < TabsCount; tab++)
        animatorScreen->switchScreen(tab, currentScreens[tab]);

#if MANGL_DEBUG_STATS_FPS
    statsDisplay = StatsDisplay::instance();
    if (!statsScene) {
        statsScene = renderer()->newScene();
    }

    auto res = Resources::instance();
    renderer()->loadAtlas(RESID_TEX_ANIMATOR, res->path(RESID_TEX_ANIMATOR), res->path(RESID_ATLAS_ANIMATOR), &displayTex);
#endif

    AnimatorMode::instance()->onUpdate();
}

void App::onRelease()
{
    for (int tab = 0; tab < TabsCount; tab++)
        currentScreens[tab] = animatorScreen->currentScreen(tab);

    switchScreen(0);
}


void App::onScreenStart(Timestamp timestamp)
{
#if MANGL_DEBUG_STATS_FPS
    statsDisplay->createScene(statsScene);
#endif
}

void App::onScreenTimer(Timestamp timestamp)
{
#if MANGL_DEBUG_STATS_FPS
    statsDisplay->updateStats(timestamp);
#endif
}

}  // namespace animator
