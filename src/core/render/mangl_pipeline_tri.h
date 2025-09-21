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

MInlineAuto pipelineRenderTriangle(RenderContextA ctx) noexcept
{
    auto& prim = *ctx.prim;

    RenderVertex tris[3]{};

    if (prim.textureFlag_) {

        auto texCoordPtr = prim.texture_.coords + prim.textureIndex_*4;
        auto origTex = RectGL::make(std::span{texCoordPtr, 4});

        //
        // Texture coordinates
        //
        auto tex = origTex;
        for (int i = 0; i < 3; i++) {
            tris[i].texture.x = tex.x + tex.w * prim.texturePoints_[i].x;
            tris[i].texture.y = tex.y + tex.h * prim.texturePoints_[i].y;

            // flip the coordinates vertically
            tris[i].texture.y += origTex.h;
            tris[i].texture.y -= tex.h;

            // convert coordinates to 0.0-1.0 format
            //tris[i].texture.x /= pipelineTextureSize_.w;
            //tris[i].texture.y /= pipelineTextureSize_.h;
        }

    }

    beginTriangles(3);


    for (int i = 0; i < 3; ++i) {
        tris[i].pos = {.x = ctx.offset.x + prim.tri[i].x, .y = ctx.offset.y + prim.tri[i].y};
        tris[i].color = pipelineColors_[i];
    }

    auto base = pushVertices(tris[0], tris[1], tris[2]);
    pushIndices(base, 0, 1, 2);

}

}  // namespace mangl::render
