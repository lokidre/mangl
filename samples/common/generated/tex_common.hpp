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

class CommonTexture final: public mangl::TextureAtlas {
    using Base = mangl::TextureAtlas;

public:
    using Texture = mangl::Texture;
    using Font = mangl::Font;
    using StrView = mangl::StrView;

    Texture button{};
    Texture buttonLabel{};
    Texture closeButton{};
    Texture backButton{};
    Texture checkbox{};
    Texture buttonCorners{};
    Texture buttonBorders{};
    Texture infoBlock{};
    Texture segmentedButtonFirst{};
    Texture segmentedButton{};
    Texture segmentedButtonLast{};
    Texture slider{};
    Texture settingsSoundsCheckbox{};
    Texture settingsMusicCheckbox{};
    Texture settingsSlider{};
    Texture dialogBackgroundBorders{};
    Texture dialogBackgroundCorners{};
    Texture dialogBackgroundFill{};
    Texture brickCover{};
    Font guiFont{};
    Font imageFont{};


    void load(StrView filename) override {
        Base::load(filename);

        button = get("button");
        buttonLabel = get("buttonLabel");
        closeButton = get("closeButton");
        backButton = get("backButton");
        checkbox = get("checkbox");
        buttonCorners = get("buttonCorners");
        buttonBorders = get("buttonBorders");
        infoBlock = get("infoBlock");
        segmentedButtonFirst = get("segmentedButtonFirst");
        segmentedButton = get("segmentedButton");
        segmentedButtonLast = get("segmentedButtonLast");
        slider = get("slider");
        settingsSoundsCheckbox = get("settingsSoundsCheckbox");
        settingsMusicCheckbox = get("settingsMusicCheckbox");
        settingsSlider = get("settingsSlider");
        dialogBackgroundBorders = get("dialogBackgroundBorders");
        dialogBackgroundCorners = get("dialogBackgroundCorners");
        dialogBackgroundFill = get("dialogBackgroundFill");
        brickCover = get("brickCover");
        guiFont = getFont("guiFont");
        imageFont = getFont("imageFont");

    }

};

