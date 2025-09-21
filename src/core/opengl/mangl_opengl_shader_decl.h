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

#if MANGL_OPENGL

namespace mangl::opengl {

struct ShaderPermutation {

    struct Bits {
        unsigned tri:1, line:1, point:1;
        unsigned tex:1;
        unsigned fog:1, shading:1;
    };

    union {
        Bits bits;
        unsigned hash;
    };
};

struct ShaderProgram;

}  // namespace mangl::opengl

#endif

