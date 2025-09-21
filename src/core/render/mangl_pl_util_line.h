/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_pl_util_global.h"
#include "mangl_pl_util_draw.h"

namespace mangl::render {

MInlineAuto pipelineRenderLineBody(RealGL width, const RenderVertex &a, const RenderVertex &b) noexcept
{

#if 0  // Drawing with line
    setUniformStateValue(&glState_.lineWidth, width);

    beginLines(1);

    auto base = pushVertices(a, b);
    pushIndices(base, 0, 1);
#endif

#if 1 // Drawing with triangles

    beginTriangles(2);

    RenderVertex tv[4] = { a, a, b, b };

    auto widthOffset = width * 0.5f * glState_.modelMtx->a[3][3] * glState_.viewMtx->a[3][3];

    // For horizontalish lines
    auto deltaX = b.pos.x - a.pos.x;
    auto deltaY = b.pos.y - a.pos.y;
    
    if (std::abs(deltaX) >= std::abs(deltaY)) {
        tv[0].pos.y -= widthOffset;
        tv[1].pos.y += widthOffset;

        tv[2].pos.y += widthOffset;
        tv[3].pos.y -= widthOffset;
    } else {
        tv[0].pos.x += widthOffset;
        tv[1].pos.x -= widthOffset;

        tv[2].pos.x -= widthOffset;
        tv[3].pos.x += widthOffset;
    }

    auto base = pushVertices(tv);

    pushIndices(base, 0, 1, 2);
    pushIndices(base, 0, 2, 3);

#endif


}

}  // namespace mangl
