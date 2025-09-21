/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_types.h"
#include "mangl_debug.h"

#include <apeal/crc/crc8.hpp>

namespace mangl {

struct Uuid {
    String generateString();
    String generateBase58();

    void generate();

private:
    uint8_t data[16]{};
};

namespace M {

//inline
//IdCommon32 generateId32() noexcept {
//    static std::atomic<IdCommon32> counter = 1;
//    auto id = counter++;
//    return (id << 8) + apl::Crc8::compute_obj((id << 8) >> 8);
//}

inline IdCommon64 generateId64() noexcept {
    static std::atomic<IdCommon64> counter = 1;
    auto id = counter++;
    return (id << 8) + apl::Crc8::compute_obj((id << 8) >> 8);
}

inline IdCommon32 generateId32() noexcept {
    return static_cast<IdCommon32>(generateId64());
}

template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
T generateId() noexcept {
    switch (sizeof(T)) {
    case 4: return static_cast<T>(generateId32());
    case 8: return static_cast<T>(generateId64());
    default:
        MDebugTrap();
        break;
    }
    return 0;
}


}  // namespace M

}  // namespace mangl

