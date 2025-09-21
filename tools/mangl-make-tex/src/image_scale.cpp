/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

void TexMake::scaleImage(const apl::Image& src, apl::Image* pdst)
{
    auto& dst = *pdst;

    dst.size = src.size * 2;
    dst.buffer.alloc(std::ssize(src.buffer) * 4);

    auto targetPtr = reinterpret_cast<uint32_t*>(dst.buffer.data());

    for (int y = 0; y < dst.size.h; ++y) {

        auto sourcePtr = reinterpret_cast<const uint32_t*>(src.buffer.data()) + y/2 * src.size.w;

        for (int x = 0; x < dst.size.w; ++x) {
            *targetPtr++ = *sourcePtr;

            if ((x % 2) == 0)
                ++sourcePtr;
        }

    }
}

