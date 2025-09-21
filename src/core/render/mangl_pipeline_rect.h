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

MInlineAuto pipelineRenderRect(RenderContextA ctx) noexcept
{
    const auto& r = ctx.prim->r;

    auto width = ctx.prim->widthFlag_ ? ctx.prim->width_ : 1.f;

    PointGL a{ctx.offset.x + r.x, ctx.offset.y + r.y};
    PointGL d{a.x + r.w, a.y + r.h};

    Point3DGL pts[4] {
        {.x = a.x, .y = a.y},
        {.x = a.x, .y = d.y},
        {.x = d.x, .y = d.y},
        {.x = d.x, .y = a.y},
    };

    pipelineRenderRectBody(ctx, false, width, pts);
}

} // namespace mangl::render
