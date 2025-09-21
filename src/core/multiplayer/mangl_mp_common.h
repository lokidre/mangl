/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_types.h"

namespace mangl::mmp {

    struct PendingFlag {
        bool active{};
        ScheduleTrigger request;

        explicit operator bool() const noexcept {  return active;  }
        void set() noexcept { request = active = true; }
        void clear() noexcept {  request = active = false;  }
    };


}  // namespace mangl::mmp

