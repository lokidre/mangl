/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_touches.h"
#include "../mangl_common.h"

#include <vector>

namespace mangl {

struct ScrollWidgetHandler {

    Cord currentOffset{};

    Cord contentsMin{}, contentsMax{};
    Cord contentsSize{};
    Cord windowSize{};

    struct Page {
        Cord start{}, size{};
    };

    std::vector<Page> pages;
    int currentPage{};
    bool pagingEnabled{};
    bool snapToEnd{};

    explicit ScrollWidgetHandler(ScrollWidgetP scroll) noexcept: scroll_{scroll} {}

    void addPage(Cord size) noexcept {
        Page p;
        p.start = pages.empty() ? 0 : pages.back().start + pages.back().size;
        p.size = size;
        pages.push_back(p);
    }

    void scrollToPage(int pageIndex);
    void scrollToPosition(Cord position);
    void scrollToBegin();
    void scrollToEnd();

    void onStart(Timestamp timestamp);
    bool onTimer(Timestamp timestamp);

    void touchesBegin(Cord position, Timestamp timestamp);
    void touchesMove(Cord position, Timestamp timestamp);
    void touchesEnd(Cord position, Timestamp timestamp);

    [[nodiscard]] bool moving() const noexcept { return moving_; }

private:
    ScrollWidgetP scroll_{};

    Cord beginTouchPosition_{}, lastTouchPosition_{};
    Timestamp lastTouchTimestamp_{};
    bool touchesBegan_{};
    bool moving_{};

    // Target moving
    bool targetMoving_{};
    bool targetMovingInitialized_{};
    Cord startPosition_{}, targetPosition_{};

    // Inertia handling
    bool inertiaMoving_ = false;
    constexpr static int InertiaMeasures = 5;
    int inertiaMeasuresCount_{};
    Timestamp inertiaVelocityMeasures_[InertiaMeasures]{};
    Timestamp inertiaSpeed_{};
    Timestamp inertiaStartTimestamp_{};


    int pageSize{};
    int prevPageIndex{-1};

    Timestamp velocity_{}, direction_{}, acceleration_{};

    Timestamp lastTimestamp_{};
    Cord eps_{0.000001};

    void scrollToPageBody(int pageIndex);
    void scrollToTarget(Cord target);

    void scrollToSnap();
};

}  // namespace mangl
