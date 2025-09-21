/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#pragma once

#include "../framework/mangl_screen_base.h"

namespace mangl {

template <class BaseScreen>
class GamePlayScreenBase: public BaseScreen {

protected:

    // Timing
    Timestamp startTimestamp_{}, currentTimestamp_{}, lastTimestamp_{};
    Timestamp pauseTimestamp_{};
    Timestamp gameTime_{}, idleTime_{};

    bool gameOver_{}, gamePaused_{};


    bool gameSleeping_{};
    Timestamp gameSleepingTimestamp_{};
    bool gameSleeping() { return gameSleeping_; }

    void gameSleep(Timestamp amount) {
        gameSleeping_ = true;
        gameSleepingTimestamp_ = currentTimestamp_ + amount;
    }


};


}  // namespace mangl
