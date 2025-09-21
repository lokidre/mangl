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

MInlineAuto pipelineRenderRectBody(RenderContextA ctx, bool filled, RealGL width, std::span<Point3DGL, 4> rp) noexcept
{
    auto& prim = *ctx.prim;

    // draw the arches first
    if (prim.radiusFlag_) {

        // Two vertices per rectangle corner
        Point3DGL orp[8]{rp[0], rp[0], rp[1], rp[1], rp[2], rp[2], rp[3], rp[3]};

        orp[0].x += prim.radius_;
        orp[1].y += prim.radius_;

        orp[2].y -= prim.radiusRightTop_;
        orp[3].x += prim.radiusRightTop_;

        orp[4].x -= prim.radiusRightBottom_;
        orp[5].y -= prim.radiusRightBottom_;

        orp[6].y += prim.radiusLeftBottom_;
        orp[7].x -= prim.radiusLeftBottom_;



        pipelineRenderCircleBody(prim, filled, {orp[0].x, orp[1].y}, pipelineColors_[0], prim.radius_, width, M::Orientation270, M::Orientation0);
        pipelineRenderCircleBody(prim, filled, {orp[3].x, orp[2].y}, pipelineColors_[1], prim.radiusRightTop_, width, M::Orientation180, M::Orientation270);
        pipelineRenderCircleBody(prim, filled, {orp[4].x, orp[5].y}, pipelineColors_[2], prim.radiusRightBottom_, width, M::Orientation90, M::Orientation180);
        pipelineRenderCircleBody(prim, filled, {orp[7].x, orp[6].y}, pipelineColors_[3], prim.radiusLeftBottom_, width, M::Orientation0, M::Orientation90);

        // now draw the remaining rectangle components
        if (filled) {
            // For filled box 5 rectangles / 10 triangles will do the job
            // We need that many to handle the coloring
            beginTriangles(12);

            auto base = pushVertices({
                // Inside rectangle
                {.pos = {.x = orp[0].x, .y = orp[1].y}, .color = pipelineColors_[0]},
                {.pos = {.x = orp[3].x, .y = orp[2].y}, .color = pipelineColors_[1]},
                {.pos = {.x = orp[4].x, .y = orp[5].y}, .color = pipelineColors_[2]},
                {.pos = {.x = orp[7].x, .y = orp[6].y}, .color = pipelineColors_[3]},

                // Outer points
                {.pos = {.x = rp[0].x, .y = orp[1].y}, .color = pipelineColors_[0]},
                {.pos = {.x = rp[1].x, .y = orp[2].y}, .color = pipelineColors_[1]},

                {.pos = {.x = orp[3].x, .y = rp[1].y}, .color = pipelineColors_[1]},
                {.pos = {.x = orp[4].x, .y = rp[2].y}, .color = pipelineColors_[2]},

                {.pos = {.x = rp[2].x, .y = orp[5].y}, .color = pipelineColors_[2]},
                {.pos = {.x = rp[3].x, .y = orp[6].y}, .color = pipelineColors_[3]},

                {.pos = {.x = orp[7].x, .y = rp[3].y}, .color = pipelineColors_[3]},
                {.pos = {.x = orp[0].x, .y = rp[0].y}, .color = pipelineColors_[0]},
            });

            pushIndices(base,
                0, 1, 2,  2, 3, 0,  // center
                4, 5, 1,  1, 0, 4,  // left
                5, 6, 7,  7, 2, 5,  // top
                8, 9, 3,  3, 2, 8,  // right
                3, 10, 11, 11, 0, 3  // bottom
                );



        } else {
            beginTriangles(4*2);

            // We need 8 more vertices (inside)
            Point3DGL irp[8];
            std::memcpy(irp, orp, sizeof(irp));

            irp[0].y += width;
            irp[1].x += width;
            irp[2].x += width;
            irp[3].y -= width;
            irp[4].y -= width;
            irp[5].x -= width;
            irp[6].x -= width;
            irp[7].y += width;


            auto base = pushVertices({
                {.pos = orp[0], .color = pipelineColors_[0] },
                {.pos = orp[1], .color = pipelineColors_[0] },
                {.pos = orp[2], .color = pipelineColors_[1] },
                {.pos = orp[3], .color = pipelineColors_[1] },
                {.pos = orp[4], .color = pipelineColors_[2] },
                {.pos = orp[5], .color = pipelineColors_[2] },
                {.pos = orp[6], .color = pipelineColors_[3] },
                {.pos = orp[7], .color = pipelineColors_[3] },

                {.pos = irp[0], .color = pipelineColors_[0] },
                {.pos = irp[1], .color = pipelineColors_[0] },
                {.pos = irp[2], .color = pipelineColors_[1] },
                {.pos = irp[3], .color = pipelineColors_[1] },
                {.pos = irp[4], .color = pipelineColors_[2] },
                {.pos = irp[5], .color = pipelineColors_[2] },
                {.pos = irp[6], .color = pipelineColors_[3] },
                {.pos = irp[7], .color = pipelineColors_[3] },
            });

            // Left
            pushIndex(base, 1, 2, 10);
            pushIndex(base, 10, 9, 1);

            // Top
            pushIndex(base, 3, 4, 12);
            pushIndex(base, 12, 11, 3);


            // Right
            pushIndex(base, 5, 6, 14);
            pushIndex(base, 14, 13, 5);

            // Bottom
            pushIndex(base, 0, 8, 15);
            pushIndex(base, 15, 7, 0);
        }

    } else {
        //
        // Use triangles to build either box or a rectangle
        //
        beginTriangles(4*2);

        int base = pushVertices(
            {.pos = rp[0], .color = pipelineColors_[0]},
            {.pos = rp[1], .color = pipelineColors_[1]},
            {.pos = rp[2], .color = pipelineColors_[2]},
            {.pos = rp[3], .color = pipelineColors_[3]}
        );

        if (filled) {
            pushIndices(base, 0, 1, 3,  3, 1, 2);

        } else {

            //
            // Using triangles to draw rectangle.
            // Always uses "inside"
            //

            // We need 4 more vertices (inside)
            pushVertices(
                {.pos = {.x = rp[0].x + width, .y = rp[0].y + width}, .color = pipelineColors_[0]},
                {.pos = {.x = rp[1].x + width, .y = rp[1].y - width}, .color = pipelineColors_[1]},
                {.pos = {.x = rp[2].x - width, .y = rp[2].y - width}, .color = pipelineColors_[2]},
                {.pos = {.x = rp[3].x - width, .y = rp[3].y + width}, .color = pipelineColors_[3]}
            );

            // Left
            pushIndices(base, 0, 1, 5,  5, 4, 0);

            // Top
            pushIndices(base, 1, 2, 5,  5, 2, 6);


            // Right
            pushIndex(base, 6, 2, 3);
            pushIndex(base, 7, 6, 3);

            // Bottom
            pushIndex(base, 0, 4, 7);
            pushIndex(base, 0, 7, 3);
        }
    }
}

}  // namespace render
