/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_rotate_effect.h"
#include "../mangl_scene.h"

namespace mangl {

RotateEffectP Scene::newRotateEffect(RotateEffectParamsA params) {
    return newEffect<RotateEffect>()->onNew(params);
}

RotateEffectP RotateEffect::onNew(RotateEffectParamsA params) {
//    if (params.prim)
//        attach(params.prim);
//
//    if (params.widget)
//        attach(params.widget);

    this->setupParams(params);

    if (params.angle > 0)
        this->setAngle(params.angle);

    //setOffset(params.offset);

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

    //this->setPermanent(params.permanent);


    return this;
}

void RotateEffect::onStart() {
    savedInfo_ = SavedInfo();

    if (auto p = attachedPrim_) {
        savedInfo_.beginRotation = p->getRotationAngle();
        savedInfo_.prim = p;
    }

    currentAngle_ = 0;
}

void RotateEffect::onStop() {
    // if (!permanent_) {
    //     auto &si = savedInfo_;
    //     if (si.prim)
    //         si.prim->setRotation(si.beginRotation);
    // }
}

void RotateEffect::onTimer(Timestamp lambda)
{
    auto rotation = maxAngle_ * lambda;

    auto &si = savedInfo_;

    if (si.prim)
        si.prim->setRotation(rotation);
}

}  // namespace mangl
