/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_setup.h"

namespace mangl {

namespace M {
constexpr Real winDefaultDpi = 96;
}  // namespace M

inline RECT winRectFromRect(RectA r) {
    RECT wrect;
    SetRect(&wrect, (DWORD)r.x, (DWORD)r.y, (DWORD)(r.x + r.w), (DWORD)(r.y + r.h));
    return wrect;
}

inline RECT winRectFromRectI(RectIA r) {
    RECT wrect;
    SetRect(&wrect, (DWORD)r.x, (DWORD)r.y, (DWORD)(r.x + r.w), (DWORD)(r.y + r.h));
    return wrect;
}

inline Rect makeRectFromRECT(const RECT& r) {
    return Rect::make(r.left, r.top, r.right - r.left, r.bottom - r.top);
}

inline RectI makeRectIFromRECT(const RECT& r) {
    return RectI::make(r.left, r.top, r.right - r.left, r.bottom - r.top);
}



inline RECT winRectFromSize(SizeA r) {
    RECT wrect;
    SetRect(&wrect, 0, 0, (DWORD)r.w, (DWORD)r.h);
    return wrect;
}

inline RECT winRectFromSizeI(SizeIA r) {
    RECT wrect;
    SetRect(&wrect, 0, 0, (DWORD)r.w, (DWORD)r.h);
    return wrect;
}

inline SizeI makeSizeIFromRECT(const RECT& r) {
    return SizeI::make(r.right - r.left, r.bottom - r.top);
}

}
