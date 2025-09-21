/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_effect.h"
#include "../mangl_texture.h"

namespace mangl {


class FramesEffect: public GuiEffectTemplate<FramesEffect> {

    using Self = FramesEffect;
    using SelfP = Self*;

private:
    int currentTextureIndex_{}, lastTextureIndex_{};
    bool bounce_{false};
    bool constrainSize_{false};


    std::vector<Texture> frames_;

    Timestamp framePeriod_{}, nextFrameTimestamp_{};
    int currentDirection_{1};
    Timestamp lastFrameTimestamp_{}, periodBeginTimestamp_{};
    int startIndex_{};

    struct SavedInfo {
        Texture texture{};
        int textureIndex{};
        MRect rect{};
        Prim *prim{};
    } savedInfo_;

public:
    Self *setFrames(TextureA tex) { return setTextures(tex); }
    auto addFrame(TextureA tex) { insertFrames(tex); return this; }
    auto addFrame(TextureA tex, int index) { frames_.push_back(tex.contentsFrame(index)); return this; }


private:
    void insertFrames(const Texture &tex)
    {
        if (tex.count == 1) {
            frames_.push_back(tex);
        } else {
            for (int i = 0; i < tex.count; ++i) {
                frames_.push_back(tex.contentsFrame(i));
            }
        }
    }

public:

    Self *setTextures(const Texture &tex)
    {
        frames_.clear();
        insertFrames(tex);
        currentTextureIndex_ = 0;
        return this;
    }

    Self *setCurrentIndex(int idx) { currentTextureIndex_ = idx; return this; }

    Self *setStartIndex(int idx) { startIndex_ = idx; return this; }

    Self *setBounce(bool flag) { bounce_ = flag; return this; }
    Self *setConstrainSize(bool flag) { constrainSize_ = flag; return this; }

protected:
    virtual void onStart() override;
    virtual void onRestart() override;
    virtual void onStop() override;
    virtual void onTimer(Timestamp lambda) override;
};

using FramesEffectP = FramesEffect*;

}  // namespace mangl

