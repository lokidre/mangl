/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_move_effect.h"

#include "../mangl_scene.h"

namespace mangl {

MoveEffectP Scene::newMoveEffect(const MoveEffectParams& params) {
    return newEffect<MoveEffect>()->onNew(params);
}

MoveEffectP MoveEffect::onNew(const MoveEffectParams& params) {
    this->setupParams(params);
    setOffset(params.offset);

    //this->setPermanent(params.permanent);

    return this;
}


void MoveEffect::onStart()
{
    savedInfo_ = SavedInfo();

    auto prepSavedInfo = [this](RectA rect, PrimP prim, WidgetP widget) {
        auto &si = savedInfo_;
        si.prim = prim;
        si.widget = widget;
        si.rect = rect;
        si.startPos = {rect.x, rect.y};
        si.endPos = si.startPos + offset_;
        if (reverse_)
            std::swap(si.endPos, si.endPos);
        si.rect.x = si.startPos.x;
        si.rect.y = si.startPos.y;
    };

    if (auto p = attachedPrim_)
        prepSavedInfo(p->getRect(), p, nullptr);
    else if (auto w = attachedWidget_)
        prepSavedInfo(w->getRect(), nullptr, w);
}


void MoveEffect::onStop()
{
    if (!permanent_) {
        auto &si = savedInfo_;
        if (si.prim) si.prim->move(si.rect); 
        else if (si.widget) si.widget->move(si.rect);
    }
}



void MoveEffect::onTimer(Timestamp lambda)
{
    auto &si = savedInfo_;
    auto p = apl::lerp(si.startPos, si.endPos, lambda);

    if (si.prim) si.prim->setPos(p); 
    else if (si.widget) si.widget->setPos(p);
}

}  // namespace mangl
