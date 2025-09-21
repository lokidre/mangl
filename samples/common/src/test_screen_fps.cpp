/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_screen.h"
#include "test_app.h"

#ifndef MANGL_CONSOLE
namespace sample {

//
// Debug stats
//
void Screen::buildRenderStats() {
    auto scene = App::getScene();
    auto safe = Env::getSafeArea();

    Color labelColor{M::Silver}, valueColor{M::LightGreen};

    Cord textGap{2};
    Cord textHeight{10};

#if MANGL_DEBUG_STATS_FPS

    //
    // Vertical
    //

    Cord currentY = safe.top() - textHeight;

    auto setupLabel = [&](StringA title) -> LabelP {
        auto label = scene->newLabel({
            .text = title,
            .pos = {safe.x, currentY},
            .color = labelColor,
            .textSize = textHeight,
            .flags = M::AlignLeft | M::AlignVCenter, //M::AlignRightCenter,
            });

        auto bbox = label->getBoundingBox();
        bbox.w = 30;

        label = scene->newLabel({
            .pos = {safe.x + bbox.w + 2, currentY},
            .color = valueColor,
            .textSize = textHeight,
            .flags = M::AlignLeftCenter,
        });

        currentY -= textHeight;

        currentY -= textGap;

        return label;
    };


    fpsLabel_ = setupLabel("FPS:");
    loadLabel_ = setupLabel("LD%:");

    runtimeLabel_ = setupLabel("RT:");
    sceneLabel_ = setupLabel("SC:");
    renderLabel_ = setupLabel("RR:");


#if 0
    //
    // Horizontal
    //
    auto setupLabel = [&](StrView title, Cord x) -> LabelP {
        auto y = safe.y + textHeight / 2 + 2;

        scene->newLabel({
            .text = title,
            .pos = {x - 1, y},
            .color = labelColor,
            .textSize = textHeight,
            .flags = M::AlignRightCenter,
            });

        return scene->newLabel({
            .pos = {x + 1, y},
            .color = valueColor,
            .textSize = textHeight,
            .flags = M::AlignLeftCenter,
            });
    };


    fpsLabel_ = setupLabel("FPS:", safe.w * 0.09);
    loadLabel_ = setupLabel("LD%:", safe.w * 0.28);

    runtimeLabel_ = setupLabel("RT:", safe.w * 0.43);
    sceneLabel_ = setupLabel("SC:", safe.w * 0.64);
    renderLabel_ = setupLabel("RR:", safe.w * 0.85);
#endif

#else
    scene->newLabel({
        .text = "MANGL_DEBUG_STATS_FPS is not defined",
        .pos = {safe.x, safe.top() - textHeight},
        .color = labelColor,
        .textSize = textHeight,
        .flags = M::AlignLeft | M::AlignVCenter, //M::AlignRightCenter,
        });

#endif

}


void Screen::updateDebugStatsScene(Timestamp timestamp) {
    // Update the gui every half seconds
    if (timestamp < fpsStatsUpdateTimestamp_)
        return;
    fpsStatsUpdateTimestamp_ = timestamp + 0.5;

    if (!fpsLabel_)
        return;

    char cbuffer[1001];
    const auto& debugStats = Framework::debugStats;

    std::snprintf(cbuffer, 100, "%0.1f", debugStats.fps());
    fpsLabel_->setText(cbuffer);

    std::snprintf(cbuffer, 100, "%0.1f", debugStats.load());
    loadLabel_->setText(cbuffer);

    std::snprintf(cbuffer, 100, "%0.4f", debugStats.runtimeLoad());
    runtimeLabel_->setText(cbuffer);

    std::snprintf(cbuffer, 100, "%0.4f", debugStats.sceneLoad());
    sceneLabel_->setText(cbuffer);

    std::snprintf(cbuffer, 100, "%0.4f", debugStats.renderLoad());
    renderLabel_->setText(cbuffer);
}

}  // namespace 
#endif
