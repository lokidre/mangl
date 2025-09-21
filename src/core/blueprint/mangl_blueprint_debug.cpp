/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#include "mangl_blueprint.h"

namespace mangl::blueprint {


static std::array debugColors {
    M::Red, M::Yellow, M::Magenta, M::Blue, 
    M::Orange, M::Lime, M::Green, M::Aqua, M::Teal, M::Purple,
};

void Engine::updateDebugStatus()
{
    if (!created_)
        return;

    auto scene = scene_;

    auto debugEnabled = debugBoundsEnabled_ || debugIdLabelsEnabled_;

    if (!debugEnabled) {
        for (auto&& node: blueprint_.orderedNodes) {
            if (node->debugBoundsPrim)
                node->debugBoundsPrim->hide();
            if (node->debugIdLabel)
                node->debugIdLabel->hide();
        }
        return;
    }


    Index colorIndex{};

    for (auto&& node: blueprint_.orderedNodes) {
        auto&& nodeRect = node->evalRect();


        if (debugBoundsEnabled_) {
            auto color = debugColors[colorIndex];

            if (!node->debugBoundsPrim)
                node->debugBoundsPrim = scene->newRect(color, nodeRect);

            node->debugBoundsPrim->unhide();
        }


        ++colorIndex;
        if (colorIndex >= std::ssize(debugColors))
            colorIndex = 0;

    }

}

}  // namespace mangl::blueprint

