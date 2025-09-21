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

class TutorialTexture final: public mangl::TextureAtlas {
    using Base = mangl::TextureAtlas;

public:
    using Texture = mangl::Texture;
    using Font = mangl::Font;
    using StrView = mangl::StrView;

    Texture tutorialImage1{};
    Texture tutorialImage2{};
    Font guiFont{};
    Font imageFont{};


    void load(StrView filename) override {
        Base::load(filename);

        tutorialImage1 = get("tutorialImage1");
        tutorialImage2 = get("tutorialImage2");
        guiFont = getFont("guiFont");
        imageFont = getFont("imageFont");

    }

};

