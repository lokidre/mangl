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

#include <apeal/random.hpp>

namespace mangl {

// Singleton random number generator

namespace M {

extern apl::Random random;


// Commonly used random generations

inline Point randomPoint(PointA mn, PointA mx) noexcept {
   Point p{};
   random.range(mn.x, mx.x, &p.x);
   random.range(mn.y, mx.y, &p.y);
   return p;
}

inline Size randomSize(SizeA mn, SizeA mx) noexcept {
   Size s{};
   random.range(mn.w, mx.w, &s.w);
   random.range(mn.h, mx.h, &s.h);
   return s;
}


inline Rect randomRect(PointA mnp, PointA mxp, SizeA mns, SizeA mxs) noexcept {
   auto p = randomPoint(mnp, mxp);
   auto s = randomSize(mns, mxs);
   return {p.x, p.y, s.w, s.h};
}



}  // namespace M
}  // namespace mangl
