/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_frames_effect.h"

namespace mangl {

void FramesEffect::onStart()
{
    savedInfo_ = SavedInfo();

    if (auto p = attachedPrim_; p) {
        auto &si = savedInfo_;
        si.prim = p;
        si.rect = p->getRect();
        si.texture = p->texture();
        si.textureIndex = p->textureIndex();
    }
}

void FramesEffect::onRestart()
{
    if (frames_.size() <= 1)
        return;

    int count = (int)frames_.size();

    if (startIndex_ >= count)
        startIndex_ = count - 1;

    if (reverse_) {
        currentTextureIndex_ = count - startIndex_ - 1;
        currentDirection_ = -1;
    } else {
        currentTextureIndex_ = startIndex_;
        currentDirection_ = 1;
    }

    framePeriod_ = duration_ / Timestamp(count);

    lastFrameTimestamp_ = currentTimestamp_;

    nextFrameTimestamp_ = currentTimestamp_ + framePeriod_;
    periodBeginTimestamp_ = currentTimestamp_;
}



void FramesEffect::onStop()
{
    auto &si = savedInfo_;
    if (permanent_) {
        if (frames_.size() <= 1)
            return;
        si.prim->setTexture(frames_[frames_.size()-1]);
    } else {
        si.prim->move(si.rect)->setTexture(si.texture, si.textureIndex);
    }
}


void FramesEffect::onTimer(Timestamp lambda)
{
    int count = (int)frames_.size();

    if (count <= 1 || duration_ <= 0)
        return;

    auto framePeriod = duration_/Timestamp(count);

    auto periodDelta = currentTimestamp_ - periodBeginTimestamp_;

    if (periodDelta >= duration_) {
        while (periodDelta >= duration_) {
            periodBeginTimestamp_ += duration_;
            periodDelta = currentTimestamp_ - periodBeginTimestamp_;
        }

        if (bounce_)
            currentDirection_ = -currentDirection_;
    }

    currentTextureIndex_ = int(periodDelta/framePeriod);

    if (currentDirection_ < 0)
        currentTextureIndex_ = count - currentTextureIndex_ - 1;

    currentTextureIndex_ += startIndex_;
    if (currentTextureIndex_ >= (int)frames_.size())
        currentTextureIndex_ -= (int)frames_.size();

    if (currentTextureIndex_ != lastTextureIndex_) {
        const auto &currentTex = frames_[currentTextureIndex_];
        auto&& texSize = currentTex.size().convert<Size>();

        auto&& si = savedInfo_;

        auto size = constrainSize_ ? si.prim->getRect().size() : texSize;

        si.prim->setTexture(currentTex);

        if (size.w != texSize.w || size.h != texSize.h) {
            auto r = si.prim->getRect();
            si.prim->move(r.x + r.w/2 - size.w/2, r.y + r.h/2 - size.h/2, size.w, size.h);
        }

        nextFrameTimestamp_ = currentTimestamp_ + framePeriod_;

        lastTextureIndex_ = currentTextureIndex_;
    }

}

}

