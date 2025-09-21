/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_fade_effect.h"

#include "../mangl_scene.h"

namespace mangl {

FadeEffectP Scene::newFadeEffect(FadeEffectParamsA params) {
    return newEffect<FadeEffect>()->onNew(params);
}

FadeEffectP Scene::newFadeInEffect(FadeEffectParamsA params) {
    return newEffect<FadeEffect>()->onNew(params)->setAlphaRange(0, 1);
}

FadeEffectP Scene::newFadeOutEffect(FadeEffectParamsA params) {
    return newEffect<FadeEffect>()->onNew(params)->setTargetAlpha(0);
}

FadeEffectP FadeEffect::onNew(const FadeEffectParams& params) {
    setTargetAlpha(params.alpha);

    // if (params.alphaFrom != 0)
    //     setRange(params.alphaFrom, params.alpha);

    this->setupParams(params);
    return this;
}


void FadeEffect::onStart()
{
    if (beginAlpha_ < 0) {
        if (attachedPrim_) beginAlpha_ = attachedPrim_->getAlpha();
        if (attachedWidget_) beginAlpha_ = attachedWidget_->getAlpha();
    } else {
        if (attachedPrim_) attachedPrim_->setAlpha(beginAlpha_);
        if (attachedWidget_) attachedWidget_->setAlpha(beginAlpha_);
    }

    // savedInfo_ = SavedInfo();
    //
    // if (auto p = attachedPrim_) {
    //     savedInfo_.alphaFlag = p->isAlpha();
    //     savedInfo_.alpha = p->getAlpha();
    //     savedInfo_.prim = p;
    // } else if (auto w = attachedWidget_) {
    //     savedInfo_.alphaFlag = w->isAlpha();
    //     savedInfo_.alpha = w->getAlpha();
    //     savedInfo_.widget = w;
    // }
}

void FadeEffect::onStop()
{
    // if (permanent_) {
    //     auto alpha = alpha_;
    //     if (reverse_) alpha = 1 - alpha;
    //
    //     //if (alpha_ == 0) {
    //     auto& si = savedInfo_;
    //     if (si.prim) si.prim->setAlpha(alpha);
    //     else if (si.widget) si.widget->setAlpha(alpha);
    //     return;
    //     //}
    //
    // }
    //
    // auto& si = savedInfo_;
    // if (si.prim) si.prim->setAlpha(si.alphaFlag, si.alpha);
    // else if (si.widget) si.widget->setAlpha(si.alphaFlag, si.alpha);
}

void FadeEffect::onTimer(Timestamp lambda)
{
    // auto& si = savedInfo_;
    //
    // auto beginAlpha = beginAlphaSet_ ? beginAlpha_ : si.alpha;
    auto effectAlpha = std::lerp(beginAlpha_, targetAlpha_, lambda);

    if (attachedPrim_) attachedPrim_->setAlpha(effectAlpha);
    if (attachedWidget_) attachedWidget_->setAlpha(effectAlpha);
}

}  // namespace mangl
