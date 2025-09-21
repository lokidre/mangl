/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/texture.h>


class SampleTexture: public mangl::TextureAtlas {
    using Base = mangl::TextureAtlas;

public:
    using Texture = mangl::Texture;

    Texture lenna{};



public:
    void load(StrView filename) {
        Base::load(filename);

        lenna = get("lenna");
    }

//private:
//    mangl::TextureAtlas tex_;

};

