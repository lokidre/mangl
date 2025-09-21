/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "animator_types.h"

#include <apeal/singleton.hpp>

#include <mangl/widgets.h>

#include "app.h"

namespace animator {

class StatsDisplay: public apl::Singleton<StatsDisplay> {
public:
    void createScene(SceneP scene);
    void updateStats(Timestamp timestamp) noexcept {
#if MANGL_DEBUG_STATS_FPS
        if (timestamp >= updateTimestamp_) {
            fpsDisplay_->setText("FPS %.2f", framework_->debug_stats_fps_);
            loadDisplay_->setText("Load %.2f%%", framework_->debug_stats_load_);

            loadRuntimeDisplay_->setText("L.RunTime %.2f%%", framework_->debug_stats_load_game_);
            loadSceneDisplay_->setText("L.Scene %.2f%%", framework_->debug_stats_load_scene_);
            loadRenderDisplay_->setText("L.Render %.2f%%", framework_->debug_stats_load_render_);

            updateTimestamp_ = timestamp + 0.5; // update the screen every half-second
        }
#endif
    }


private:
    LabelP fpsDisplay_, loadDisplay_;
    LabelP loadRuntimeDisplay_, loadSceneDisplay_, loadRenderDisplay_;
    Timestamp updateTimestamp_{};
    App* framework_{};
};

}  // namespace animator
