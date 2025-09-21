/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "stats_display.h"

#include "app.h"

namespace animator {

void StatsDisplay::createScene(SceneP scene)
{
#if MANGL_DEBUG_STATS_FPS
    updateTimestamp_ = 0;

    scene->release();

    framework_ = App::instance();

    const auto& font = framework_->guiTex.font("displayFont");
    auto width = framework_->layout.w;
    auto height = 12. * framework_->scale_value_; //font.height() ;

    LabelP* labels[] = {
        &fpsDisplay_, &loadDisplay_, &loadRuntimeDisplay_, &loadSceneDisplay_, &loadRenderDisplay_
    };

    float y = 0;
    for (auto& l : labels) {
        *l = scene->newWidget<Label>()->setFont(font)->setHeight(height)->move(0, y)->create();
        y += height;
    }
#endif
}

}  // namespace animator
