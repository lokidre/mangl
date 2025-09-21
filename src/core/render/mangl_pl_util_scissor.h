/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_render.h"

namespace mangl::render {

MInlineAuto processScissor(RenderContextA ctx) noexcept
{
    if (ctx.prim->scissorFlag_) {
        auto sr = ctx.prim->scissorRect_;

        sr.x += ctx.offset.x;
        sr.y += ctx.offset.y;

        //glState_.scissor = true;
        //glState_.scissorRect = sr;

        setUniformStateValue(&glState_.scissor, true);
        setUniformStateValue(&glState_.scissorRect, sr);
    } else {
        setUniformStateValue(&glState_.scissor, false);
        //glState_.scissor = false;
    }
}

}  // namespace mangl::opengl

