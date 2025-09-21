/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_crossfade_effect.h"
#include "../mangl_scene.h"

namespace mangl {

CrossfadeEffectP Scene::newCrossfadeEffect(const CrossfadeEffectParams& params) {
    return newEffect<CrossfadeEffect>()->onNew(params);
}


CrossfadeEffectP CrossfadeEffect::onNew(const CrossfadeEffectParams& params) {
    setColors(params.color1, params.color2);
    this->setupParams(params);
    return this;
}


void CrossfadeEffect::onStart()
{
    savedInfo_ = SavedInfo();

    if (attachedPrim_) {
        savedInfo_.colorFlag = attachedPrim_->isColor();
        savedInfo_.color = attachedPrim_->getColor();
        savedInfo_.prim = attachedPrim_;
    } else if (attachedWidget_) {
        savedInfo_.colorFlag = attachedWidget_->isColor();
        savedInfo_.color = attachedWidget_->getColor();
        savedInfo_.widget = attachedWidget_;
    }

}

void CrossfadeEffect::onStop()
{
    if (permanent_)
        return;

    auto& si = savedInfo_;

    if (si.colorFlag) {
        if (si.prim)
            si.prim->setColor(si.color);
        else if (si.widget)
            si.widget->setColor(si.color);
    } else {
        if (si.prim)
            si.prim->clearColor();
        else if (si.widget)
            si.widget->clearColor();
    }
}

void CrossfadeEffect::onTimer(Timestamp l)
{
//    Timestamp progress = (currentTimestamp_ - startTimestamp_) / duration_;
//    Timestamp cycles;
//
//    Timestamp lambda = std::modf(progress / crossfadePeriod_, &cycles);
//
//    int startIndex{0}, endIndex{1};
//
//    if (lambda >= 0.5) {
//        lambda = 1. - lambda;
//    }
//
//    lambda *= 2;

    int startIndex{0}, endIndex{1};
    auto lambda = l;

    auto effectColor = lerp(colors_[startIndex], colors_[endIndex], lambda);

    if (attachedPrim_)
        attachedPrim_->setColor(effectColor);
    else if (attachedWidget_)
        attachedWidget_->setColor(effectColor);
}

}  // namespace mangl
