/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_scene.h"
#include "../widgets/mangl_widget.h"
#include "../effects/mangl_effect.h"

namespace mangl {

void Scene::onTimer(Timestamp timestamp) noexcept
{
    M::debugAssertMainThread();

    // Process all the scene controls
    //widgetsEntered_ = true;
    //for (auto w: widgets_) {
    for (auto w = widgets_; w; w = w->next) {
        if (w->disabled_ || !w->visible_)
            continue;

//        if (!w->created_) {
//            w->create();
//            //debug_printf("Scene::onTimer:w is not created");
//        }

        if (!w->created_)
            continue;
        w->onTimer(timestamp);
    }
    //widgetsEntered_ = false;

    // Apply all the effects
    for (auto ef: allocatedEffects_) {
        if (ef->active_)
            ef->processTimer(timestamp);
    }

}

}  // namespace mangl
