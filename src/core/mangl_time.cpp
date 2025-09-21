/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_time.h"

#include <thread>
#include <chrono>


namespace mangl {
namespace M {

void sleep(Timestamp seconds) {
    auto ms = (unsigned long long)(seconds * 1000000);
    std::this_thread::sleep_for(std::chrono::microseconds(ms));
}

}  // namespace M
}  // namespace mangl
