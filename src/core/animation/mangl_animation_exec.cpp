/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_animation.h"

#include <apeal/time.hpp>
#include <apeal/parse.hpp>
#include <apeal/file.hpp>

namespace mangl {


void Animation::start(Timestamp timestamp)
{
    // check screenLayout
    if (layoutFlag) {
    } else {
        layout = viewport_;
        //screenLayout = padding_ ;
        //if ( padded ) {
        //	screenLayout = paddedViewport_ ;
        //}
        //screenLayout.w = viewport_.w ;
        //screenLayout.h = viewport_.h ;
    }

    started = true, finished = false;
    startTimestamp_ = restartTimestamp_ = timestamp;

    // stop all the layers
    for (auto &layer: layers) {
        for (auto &keyframe: layer.keyframes)
            keyframe.setSpriteVisible(false);
        layer.finished = layer.started = false;
    }

#if _DEBUG
    if (debugVerbose_) {
        MDebugFunc("animation : start : %s : %f", filename_, timestamp);

        MDebugPrint("    variables: ");
        for (const auto &var: variables) {
            MDebugPrint("        %s: %s", var.first, var.second);
        }
    }
#endif

    setupParameters();

}

}

