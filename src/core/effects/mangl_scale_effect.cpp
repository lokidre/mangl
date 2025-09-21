/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_scale_effect.h"

#include "../mangl_scene.h"

namespace mangl {


ScaleEffectP Scene::newScaleEffect(const ScaleEffectParams& params) {
    return newEffect<ScaleEffect>()->onNew(params);
}

ScaleEffectP ScaleEffect::onNew(const ScaleEffectParams& params) {
//    if (params.prim)
//        attach(params.prim);
//
//    if (params.widget)
//        attach(params.widget);

    this->setupParams(params);
    setScale(params.scale);

//    if (params.easing != Easing::None)
//        this->setEasing(params.easing);
//
//
//    if (params.duration != 0)
//        this->setDuration(params.duration);
//
//    if (params.period != 0)
//        this->setPeriod(params.period);
//
//    this->setRepeat(params.repeat);
//    this->setBounce(params.bounce);

    this->setPermanent(params.permanent);


    return this;
}

void ScaleEffect::onStart()
{
    savedInfo_ = SavedInfo();

    if (auto p = attachedPrim_) {
        auto& si = savedInfo_;
        si.r = p->getRect();
        si.prim = p;
    }

    beginScale_ = 1;
    endScale_ = scale_;
}

void ScaleEffect::onStop()
{
    auto &si = savedInfo_;
    if (si.prim)
        si.prim->move(si.r);
}


void ScaleEffect::onTimer(Timestamp lambda)
{
    auto currentScale = std::lerp(beginScale_, endScale_, lambda);

    auto &si = savedInfo_;
    if (si.prim) {
        auto size = si.r.size() * currentScale;
        si.prim->move(si.r.centered(size));
    }
}

}  // namespace mangl
