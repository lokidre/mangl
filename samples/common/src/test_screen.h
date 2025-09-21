/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "test_types.h"


#ifndef MANGL_CONSOLE

#include <mangl/screen.h>
#include <mangl/widgets.h>
#include <mangl/game.h>

namespace sample {

class Screen: public GameScreenBase {
    using Base = GameScreenBase;



protected:

    //
    // Background
    //
    void buildSampleBackground();
    void sampleBuildBackground() { buildSampleBackground(); }
    void sampleBuildTitle(StrView title);

    //
    // Status Bar
    //
private:
    LabelP testStatusLabel_{};

protected:
    Cord testStatusTextSize_{12};
    Size testStatusTextMargins_{2, 2};

    Rect buildStatusBar();
    Rect sampleBuildStatusBar() { return buildStatusBar(); }

    void setStatusMessage(StrView msg) {
        if (!testStatusLabel_) buildStatusBar();
        testStatusLabel_->setText(msg);
    }

    void sampleSetStatusMessage(StrView msg) { setStatusMessage(msg); }


    template <class ...Args>
    void setStatusMessage(StrView fmt, Args&&... args) noexcept {
        if (!testStatusLabel_) buildStatusBar();
        testStatusLabel_->setText(fmt, std::forward<Args>(args)...);
    }

    template <class ...Args>
    void sampleSetStatusMessage(StrView fmt, Args&&... args) noexcept {
        if (!testStatusLabel_) buildStatusBar();
        testStatusLabel_->setText(fmt, std::forward<Args>(args)...);
    }


    //
    // FPS measurements
    //
private:
    LabelP fpsLabel_{}, loadLabel_{}, runtimeLabel_{}, renderLabel_{}, sceneLabel_{};
    Timestamp fpsStatsUpdateTimestamp_{};

protected:
    void buildRenderStats();
    void updateDebugStatsScene(Timestamp timestamp);


    //
    // Sample Layout
    //
    struct GridParams {
        int cols{2};
        int direction{1};
    };

    void gridStart(const GridParams& p);
    void sampleGridStart(const GridParams& p) { gridStart(p); }

    Rect gridAddButton();
    Rect sampleGridAddButton() { return gridAddButton(); }

    ButtonP gridAddButton(ButtonParams p);
    ButtonP sampleGridBuildButton(ButtonParams p) {
        return gridAddButton(p);
    }

    Rect gridAddScreenButton();
    ButtonP gridAddScreenButton(ButtonParams p);
    ButtonP sampleGridBuildScreenButton(ButtonParamsA p) { return gridAddScreenButton(p); }

    Point sampleGridLayoutScreenLabel();
    LabelP sampleGridAddScreenLabel(LabelParams p);

    Rect gridAddSlider();
    Rect gridAddCheckbox(StringA title = {});
    void gridAdvanceCol();
    void sampleGridAdvanceRow();


    //int gridColumns_{2};
    Cord gridStartTopY_{0.9};
    Cord gridStartBottomY_{0.1};
    Cord currGridY_{0.9};
    int currGridCol_{};
    //int currGridRow_{};
    GridParams gridParams_{};


protected:
    void onTimerPostfix(Timestamp timestamp) override;

};

}  // namespace

#endif

