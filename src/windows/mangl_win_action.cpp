/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_main_window.h"

#if MANGL_WINMAIN


#include "../core/mangl_framework.h"

namespace mangl {

void ManglMainWindow::processFrameworkActions(HDC hdc)
{
    auto fw = Framework::instance();

    if (fw->frameworkActionQueueIndex_ == 0)
        return;

    for (int queueIndex = fw->frameworkActionQueueBegin_; queueIndex < fw->frameworkActionQueueIndex_; ++queueIndex) {

        fw->frameworkActionQueueBegin_ = queueIndex + 1;

        const auto &action = fw->frameworkActionQueue_[queueIndex];

        if (processFrameworkActionsGui(hdc, action))
            continue;
    }

    fw->frameworkActionQueueBegin_ = fw->frameworkActionQueueIndex_ = 0;
}

}

#endif
