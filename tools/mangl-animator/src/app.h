/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/audio.h>
#include <mangl/framework.h>
#include <mangl/texture.h>

#include "animator.h"

namespace animator {

class AnimatorScreen;
class StatsDisplay;

class App: public AppBase<App> {
    using Base = AppBase<App>;

public:
    TextureAtlas guiTex, displayTex;
    TextureAtlas backgroundTex, animationTex, layoutTex, modelTex;

    AnimatorScreen* animatorScreen{};

    String currentScreens[TabsCount];
    StatsDisplay* statsDisplay{};
    Scene* statsScene{};


protected:
    void onInit() override;
    void onStart() override;
    void onRelease() override;
    void onScreenStart(Timestamp timestamp) override;
    void onScreenTimer(Timestamp timestamp) override;
};

}  // namespace animator

