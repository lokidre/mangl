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


MInlineAuto pipelineRenderLine(RenderContextA ctx) noexcept
{
    auto& prim = *ctx.prim;
    auto width = prim.widthFlag_ ? prim.width_ : 1.f;
    const auto &f = prim.r;

    auto a = prim.r.position() + ctx.offset;
    PointGL b{a.x + f.w, a.y + f.h};

    // Calling the shader to draw the line

    // Move the first vertices off
    auto t = b - a;  // tangent vector
    PointGL r{-t.y, t.x};  // rotate by 90 deg
    const auto &tu = pipelineTextureA_, &tv = pipelineTextureD_;
    auto u = r;

    RenderVertex va{.pos = {.x = a.x, .y = a.y}, .color = pipelineColors_[0], .texture = {tu.x, tu.y}, .normal = {.x = u.x, .y = u.y}};
    RenderVertex vb{.pos = {.x = b.x, .y = b.y}, .color = pipelineColors_[2], .texture = {tv.x, tv.y}, .normal = {.x = u.x, .y = u.y}};

    pipelineRenderLineBody(width, va, vb);
}


}  // namespace mangl::render

