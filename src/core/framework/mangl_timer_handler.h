/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"

namespace mangl {

struct TimerHandler {

    using TimerId = CommonId;
    using TimerFunc = std::function<void (Timestamp)>;


    CommonId timerId{};

    [[nodiscard]] bool isRegistered() const noexcept { return timerId != 0; }

    CommonId registerHandler(TimerFunc&& func) noexcept;
    CommonId unregister() noexcept;

    ~TimerHandler() noexcept {
        unregister();
    }
};


}  // namespace mangl

