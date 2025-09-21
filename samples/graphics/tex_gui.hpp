/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
// Automatically Generated. DO NOT EDIT
#pragma once

#include <mangl/texture.h>

namespace chessdash {

class GuiTexture final: public mangl::TextureAtlas {
    using Base = mangl::TextureAtlas;

public:
    using Texture = mangl::Texture;
    using Font = mangl::Font;
    using StrView = mangl::StrView;

    Texture homeTitle{};
    Texture homePlayButtonLabel{};
    Texture homeSoundsCheckbox{};
    Texture homeSettingsButton{};
    Texture homeTutorialButton{};
    Texture backgroundPaperElements{};
    Texture navBackButton{};
    Texture navCloseButton{};
    Texture gameStartButtonLabel{};
    Texture screenButtonIconVsComp{};
    Texture screenButtonIconOneDevice{};
    Texture screenButtonIconDash{};
    Texture playerColorSelect{};
    Texture difficultyDirectionLeft{};
    Texture difficultyDirectionRight{};


    void load(StrView filename) override {
        Base::load(filename);

        homeTitle = get("homeTitle");
        homePlayButtonLabel = get("homePlayButtonLabel");
        homeSoundsCheckbox = get("homeSoundsCheckbox");
        homeSettingsButton = get("homeSettingsButton");
        homeTutorialButton = get("homeTutorialButton");
        backgroundPaperElements = get("backgroundPaperElements");
        navBackButton = get("navBackButton");
        navCloseButton = get("navCloseButton");
        gameStartButtonLabel = get("gameStartButtonLabel");
        screenButtonIconVsComp = get("screenButtonIconVsComp");
        screenButtonIconOneDevice = get("screenButtonIconOneDevice");
        screenButtonIconDash = get("screenButtonIconDash");
        playerColorSelect = get("playerColorSelect");
        difficultyDirectionLeft = get("difficultyDirectionLeft");
        difficultyDirectionRight = get("difficultyDirectionRight");

    }

};

}  // namespace chessdash

