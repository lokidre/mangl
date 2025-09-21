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

MInlineAuto pipelineRenderCircle(RenderContextA ctx) noexcept
{
    auto& prim = *ctx.prim;
    auto radius = prim.radius_;
    auto width = prim.widthFlag_ ? prim.width_ : RealGL{1};

    auto c = ctx.offset + prim.tri[0];

    pipelineRenderCircleBody(prim, false, c, radius, width, prim.beginSegment_, prim.endSegment_);
}


MInlineAuto pipelineRenderDisk(RenderContextA ctx) noexcept
{
    auto& prim = *ctx.prim;
    auto radius = prim.radius_;
    auto width = prim.widthFlag_ ? prim.width_ : RealGL{1};

    auto c = ctx.offset + prim.tri[0];

    pipelineRenderCircleBody(prim, true, c, radius, width, prim.beginSegment_, prim.endSegment_);
}


}  // namespace mangl::render
