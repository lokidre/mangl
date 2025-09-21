/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"

namespace mangl {

enum class Easing {
    None,
    CubicInOut,
    QuartInOut,
    Invalid,
};

namespace M {
constexpr Real easeCubicInOut(Real t) noexcept {
    if (t <= 0.5)
        return 4 * t * t * t;

    t -= 1;
    return 1 + t * t * t * 4;
}

constexpr Real easeQuartInOut(Real t) noexcept {
    if (t <= 0.5) {
        t *= t;           // square t
        return 8 * t * t; // we have 0.5
    }

    t -= 1;
    t *= t;
    return 1 - 8 * t * t;
}

constexpr Real ease(Easing easing, Real t) noexcept {
    switch (easing) {
        using enum Easing;
    case None: return t;
    case CubicInOut: return easeCubicInOut(t);
    case QuartInOut: return easeQuartInOut(t);
    default:
        //MDebugTrap();
        break;
    }
    return t;
}
} // namespace M

} // namespace mangl
