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

MInlineAuto pipelineRenderPoint(RenderContextA ctx) noexcept
{
    auto prim = ctx.prim;

    auto width = prim->widthFlag_ ? prim->width_ : 1.f;

    glState_.lineWidth = width;


    PointGL p{ctx.offset.x + prim->r.x, ctx.offset.x + prim->r.y};

    RenderVertex v{.pos = {.x = p.x, .y = p.y}, .color = pipelineColors_[0]};

    beginPoints(1);

    auto base = pushVertex(v);
    pushIndices(base, 0);
}

}  // namespace mangl::render
