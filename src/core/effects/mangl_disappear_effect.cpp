/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_disappear_effect.h"
#include "../mangl_scene.h"

namespace mangl {


DisappearEffectP Scene::newDisappearEffect(const DisappearEffectParams& params) {
    return newEffect<DisappearEffect>()->onNew(params);
}

DisappearEffectP DisappearEffect::onNew(const DisappearEffectParams& params) {
//    if (params.prim)
//        attach(params.prim);
//
//    if (params.widget)
//        attach(params.widget);

    this->setupParams(params);
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

    this->setPermanent(params.permanent);


    return this;
}


void DisappearEffect::onStop() {
    if (attachedPrim_) attachedPrim_->setVisible(reverse_);
    if (attachedWidget_) attachedWidget_->setVisible(reverse_);
}


} //namespace mangl
