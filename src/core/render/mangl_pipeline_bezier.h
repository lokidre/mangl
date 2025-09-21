/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_pl_util_header.h"

namespace mangl::render {

//
// Bezier curve is given by 4 points. a, b - beginning and end, and u, v - control points
//
// prim.r.(x,y) - a, prim.r.(pos+size) - b
// tri[0]. tri[1] - control points
//

MInlineAuto pipelineRenderBezier(RenderContextA ctx) noexcept
{
    auto& prim = *ctx.prim;

    using cognit::tiny;

    using std::hypot;
    using std::floor;

    auto width = prim.widthFlag_ ? prim.width_ : RealGL{1};
    auto& primFrame = prim.r;

    auto ax = primFrame.x + ctx.offset.x, ay = primFrame.y + ctx.offset.y, dx = primFrame.w, dy = primFrame.h;
    auto bx = prim.tri[0].x, by = prim.tri[0].y;
    auto cx = prim.tri[1].x, cy = prim.tri[1].y;

    // Determine number of segments
    // Calculate the maximum curve length, the length can't be more than control path
    auto l1 = hypot(bx-ax, by-ay);
    auto l2 = hypot(cx-bx, cy-by);
    auto l3 = hypot(dx-cx, dy-cy);
    auto total = l1 + l2 + l3;
    constexpr RealGL pix{16};

    auto fsegments = floor(total/pix);
    if (fsegments < 2)
        fsegments = 2;

    //fsegments = 3 ;

    int segments = int(fsegments);
    constexpr auto epsilon = cognit::constants::epsilon<RealGL>;

    auto calculateBezier = [&ax, &ay, &bx, &by, &cx, &cy, &dx, &dy](RealGL t, auto& px, auto& py, auto& tx, auto& ty) {
        const auto mt = 1 - t;
        const auto tt = t*t;
        const auto mtmt = mt*mt;
        const auto mtmtmt = mtmt * mt;
        const auto ttt = tt * t;

        auto a = mtmtmt, b = mtmt*t*3, c = mt*tt*3, d = ttt;

        px = a*ax + b*bx + c*cx + d*dx;
        py = a*ay + b*by + c*cy + d*dy;

        // calculate tangent - derivative
        // -3(1-t)^2 * A - 6t(1-t) * B + 3(1-t)^2 * B  - 3t^2 * C + 6t(1-t) * C + 3t^2 * D ;
        a = -3*mtmt;
        b = -6*t*mt + 3*mtmt;
        c = -3*tt + 6*t*mt;
        d = 3*tt;

        tx = a*ax + b*bx + c*cx + d*dx;
        ty = a*ay + b*by + c*cy + d*dy;
    };

    auto adjustTangent = [&width, &ax, &ay, &dx, &dy, &epsilon](auto tx, auto ty, const auto x, const auto y, auto& ux, auto& uy, auto& vx, auto& vy) {

        if (tiny(tx, epsilon) && tiny(ty, epsilon)) {
            tx = dx-ax, ty = dy-ay;
        }

        auto n = std::hypot(tx, ty);

        auto r = PointGL{-ty, tx};  // rotate by 90 deg
        auto width_2 = width/2;
        auto l = width_2 / n;
        auto u = PointGL{r.x * l, r.y * l};

        ux = x + u.x, uy = y + u.y;
        vx = x - u.x, vy = y - u.y;
    };

    RealGL t, px, py, tx, ty;
    auto vU = RenderVertex{.color = pipelineColors_[0], .texture = pipelineTextureA_};
    auto vV = RenderVertex{.color = pipelineColors_[0], .texture = {pipelineTextureD_.x, pipelineTextureA_.y}};  // very first vertex

    calculateBezier(0, px, py, tx, ty);
    adjustTangent(tx, ty, ax, ay, vU.pos.x, vU.pos.y, vV.pos.x, vV.pos.y);

    beginTriangleVertices(2 + segments*2, segments*6);

    auto base = pushVertices(vU, vV);

    for (int seg = 1; seg <= segments; ++seg) {

        t = RealGL(seg)/fsegments;
        calculateBezier(t, px, py, tx, ty);
        adjustTangent(tx, ty, px, py, vU.pos.x, vU.pos.y, vV.pos.x, vV.pos.y);

        if (prim.textureFlag_) {
            vU.texture.y = (1-t)*pipelineTextureA_.y + t*pipelineTextureD_.y;
            vV.texture.y = vU.texture.y;
        }

        pushVertices(vU, vV);

        pushIndices(base, 0, 1, 3);
        pushIndices(base, 0, 3, 2);

        base += 2;
    }

}

}  // namespace mangl::render
