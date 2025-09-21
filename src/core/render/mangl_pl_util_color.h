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

MInlineVoid processPipelineColors(const RenderContext& ctx) noexcept
{
    auto& prim = *ctx.prim;

    // Start with white
    for (auto& c: pipelineColors_) {
        c.r = c.g = c.b = c.a = 1;
    }

    // Set colors from the prim
    if (prim.colorFlag_) {
        pipelineColors_[0] = prim.color_;
        if (prim.colorFlag2_) {
            pipelineColors_[1] = prim.color2_;
            if (prim.colorFlag3_) {
                pipelineColors_[2] = prim.color3_;
                pipelineColors_[3] = prim.colorFlag4_ ? prim.color4_ : pipelineColors_[2];
            } else {
                pipelineColors_[2] = pipelineColors_[3] = pipelineColors_[1];
            }
        } else {
            pipelineColors_[1] = pipelineColors_[2] = pipelineColors_[3] = pipelineColors_[0];
        }
    }

    if (prim.alphaFlag_) {
        // TODO: do the parents
        for (auto &c: pipelineColors_)
            c.a = prim.alpha_;
    }


    // Apply scene effects
    if (ctx.scene->colorEffectFlag_) {
        for (auto &c: pipelineColors_) {
            c *= ctx.scene->colorEffectValue_;
        }
    }

    if (ctx.scene->alphaFlag_) {
        for (auto &c: pipelineColors_) {
            c.a *= static_cast<RealGL>(ctx.scene->alphaValue_);
        }
    }
}

}  // namespace mangl::render
