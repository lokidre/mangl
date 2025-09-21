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

void Screen::buildSampleBackground() {
    auto scene = backgroundScene_;
    //auto& sbody = M::Styles::body;

    scene->newBox(M::Black, Env::getFullArea());
    scene->newBox(0x1e1f22, Env::getSafeArea());
}

void Screen::sampleBuildTitle(StrView title) {
    auto scene = backgroundScene_;
    auto safe = Env::getSafeArea();

    scene->newLabel({
        .text = String{title},
        .pos = safe.center().updY(safe.top() - 20),
        .flags = M::AlignCenter | M::AlignTop,
    });

    gridStartTopY_ = 0.8;
}



Rect Screen::buildStatusBar() {
    auto scene = backgroundScene_;
    auto safe = Env::getSafeArea();

    auto& sbody = M::Styles::body;


    Cord statusTextSize = 12;
    Margins statusTextMargin{.h = 2, .v = 6};

    Color statusBarColor{0x2b2d30};
    Color statusTextColor{0x9a9fa8};

    //auto statusBarColor = sbody.backgroundColor.adjustedBrightness(0.2);

    Rect statusBarRect = safe;
    statusBarRect.h = statusTextSize + statusTextMargin.top() + statusTextMargin.bottom();

    scene->newBox(statusBarColor, statusBarRect);

    testStatusLabel_ = scene->newLabel({
        .pos = {statusTextMargin.left(), statusBarRect.y + statusBarRect.h * 0.5},
        .color = statusTextColor,
        .textSize = statusTextSize,
        .flags = M::AlignLeft | M::AlignVCenter,
    });

    return statusBarRect;
}



void Screen::onTimerPostfix(Timestamp timestamp) {
    Base::onTimerPostfix(timestamp);

    if (fpsLabel_)
        updateDebugStatsScene(timestamp);
}
}  // namespace 

#endif
