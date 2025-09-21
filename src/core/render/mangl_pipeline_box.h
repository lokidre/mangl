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

MInlineAuto pipelineRenderBox(RenderContext &ctx) noexcept
{
    auto &f = ctx.prim->r;

    PointGL a{ctx.offset.x + f.x, ctx.offset.y + f.y};
    PointGL d{a.x + f.w, a.y + f.h};

    Point3DGL pts[4] {
        {.x = a.x, .y = a.y},
        {.x = a.x, .y = d.y},
        {.x = d.x, .y = d.y},
        {.x = d.x, .y = a.y},
    };

    pipelineRenderRectBody(ctx, true, 0, pts);
}

}  // namespace mangl::render
