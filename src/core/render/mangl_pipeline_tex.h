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

MInlineAuto pipelineRenderTexture(RenderContext &ctx) noexcept
{
    using namespace apl;

    auto& prim = *ctx.prim;

    //
    // Handle tiling
    //
    int elementsTotal{1};
    int elementsCols{1}, elementsRows{1};
    auto texCoordPtr = prim.texture_.coords + prim.textureIndex_ * 4;
    auto origTex = RectGL::make(std::span{texCoordPtr, 4});
    auto primElSize = prim.elSize_;
    auto primRect = prim.r;
    auto elementSize = origTex.size() / prim.texture_.density;

    if (primElSize.w && primElSize.h) {
        elementSize = primElSize;
        if (!elementSize.w)
            elementSize.w = elementSize.h * origTex.w / origTex.h;
        else if (!elementSize.h)
            elementSize.h = elementSize.w * origTex.h / origTex.w;
    }


    if (prim.textureScale_ != 0) {
        elementSize *= prim.textureScale_ ;
    }


    if (prim.tiledVert_ || prim.tiledHorz_) {

        if (prim.tiledHorz_) {
            elementsCols = int(primRect.w / elementSize.w);
            if (elementsCols * elementSize.w < primRect.w)
                elementsCols++;
        }

        if (prim.tiledVert_) {
            elementsRows = int(primRect.h / elementSize.h);
            if (elementsRows * elementSize.h < primRect.h)
                elementsRows++;
        }

        elementsTotal = elementsRows * elementsCols;
    }

    Point3DGL p[4]{};

    auto elementHeight = prim.tiledVert_ ? elementSize.h : primRect.h;
    auto elementWidth = prim.tiledHorz_ ? elementSize.w : primRect.w;

    RealGL elementHeightAdvance = 0, elementWidthAdvance = 0;
    RealGL elementYAdvance = elementHeight;
    RealGL elementXAdvance = elementWidth;
    auto elementXMax = primRect.w;
    auto elementYMax = primRect.h;
    RealGL elementXBegin{}, elementYBegin {};

    auto origElementHeight = elementHeight;
    auto origElementWidth = elementWidth;


    if (prim.blurFlag_) {
        RealGL blurStep = 2;
        elementsTotal = (int)(prim.blur / blurStep);

        elementXAdvance = blurStep;
        elementYAdvance = blurStep;
        elementHeightAdvance = -elementYAdvance * 2;
        elementWidthAdvance = - elementXAdvance * 2;

        elementYMax = 0; //prim.blur;

        elementXBegin = -prim.blur;
        elementYBegin = -prim.blur;
        elementWidth = primRect.w + prim.blur * 2;
        elementHeight = primRect.h + prim.blur * 2;

        elementXMax = elementXBegin + elementXAdvance; //prim.blur;

        //elementYMax = elementYBegin + elementYAdvance;

    }


    for ( auto elementY = elementYBegin; elementY < elementYMax; elementY += elementYAdvance, elementHeight += elementHeightAdvance) {
        auto tex = origTex;

        //elementWidth = origElementWidth;

        if (prim.blurFlag_) {
            elementXBegin = elementY;
            elementXMax = elementXBegin + elementXAdvance;
        }

        elementWidth = origElementWidth;

        for (auto elementX = elementXBegin; elementX < elementXMax; elementX += elementXAdvance, elementWidth += elementWidthAdvance) {

            beginTriangles(2);

            // prepare the screen coordinates
            tex.x = origTex.x;
            tex.w = origTex.w;

            //
            // object coordinates
            //
            if (elementsTotal == 1 && !prim.tiledHorz_ && !prim.tiledVert_ && !prim.blurFlag_) {
                p[0] = {.x = primRect.x + ctx.offset.x, .y = primRect.y + ctx.offset.y};
                p[1] = {.x = primRect.x + ctx.offset.x + primRect.w, .y = primRect.y + ctx.offset.y};
                p[2] = {.x = primRect.x + ctx.offset.x + primRect.w, .y = primRect.y + ctx.offset.y + primRect.h};
                p[3] = {.x = primRect.x + ctx.offset.x, .y = primRect.y + ctx.offset.y + primRect.h};
            } else {
                p[0] = p[1] = p[2] = p[3] = {
                    .x = primRect.x + ctx.offset.x + elementX,
                    .y = primRect.y + ctx.offset.y + elementY
                };

                if (prim.blurFlag_) {
                    p[1].x += elementWidth;
                    p[2].x += elementWidth;
                    p[2].y += elementHeight;
                    p[3].y += elementHeight;
                } else {

                    if (prim.tiledHorz_) {
                        if (elementX + elementWidth > primRect.w) {
                            elementWidth = primRect.w - elementX;
                            tex.w = elementWidth;
                        }
                        p[1].x += elementWidth;
                        p[2].x += elementWidth;
                    } else {
                        p[1].x += primRect.w;
                        p[2].x += primRect.w;
                    }

                    if (prim.tiledVert_) {
                        if (elementY + elementHeight > primRect.h) {
                            elementHeight = primRect.h - elementY;
                            tex.h = elementHeight;
                        }
                        p[2].y += elementHeight;
                        p[3].y += elementHeight;

                    } else {
                        p[2].y += primRect.h;
                        p[3].y += primRect.h;
                    }
                }

            }

            // Texture coordinates (verticall flipped)
            PointGL tu{tex.x, tex.y + origTex.h};
            PointGL tv{tex.x + tex.w, tex.y + origTex.h - tex.h};

            if (prim.flipHorz_) {
                std::swap(tu.x, tv.x);
            }

            if (prim.flipVert_) {
                std::swap(tu.y, tv.y);
            }

            RenderVertex v[4]{
                {.pos = p[0], .color = pipelineColors_[0], .texture = tu},
                {.pos = p[1], .color = pipelineColors_[1], .texture = {tv.x, tu.y}},
                {.pos = p[2], .color = pipelineColors_[2], .texture = tv},
                {.pos = p[3], .color = pipelineColors_[3], .texture = {tu.x, tv.y}},
            };

            if (prim.blurFlag_) {
                auto alpha = ((prim.blur + elementY + RealGL(0.6)) / prim.blur) * prim.blurOpacity;
                alpha *= RealGL(0.4);
                v[0].color.a *= alpha;
                v[1].color.a *= alpha;
                v[2].color.a *= alpha;
                v[3].color.a *= alpha;
            }

            if (prim.rotationFlag_) {
                // center of rotation
                auto pivot = ctx.offset + primRect.center();

                auto rcos = prim.rotationCos_;
                auto rsin = prim.rotationSin_;

                for (auto &i: v) {
                    PointGL t{i.pos.x - pivot.x, i.pos.y - pivot.y};

                    i.pos.x = rcos * t.x - rsin * t.y + pivot.x;
                    i.pos.y = rsin * t.x + rcos * t.y + pivot.y;
                }
            }

            if (prim.orientationFlag_ && prim.orientation_ != M::Orientation0) {
                Point3DGL t;

                switch (prim.orientation_) {
                case M::Orientation270:
                    t = v[0].pos;
                    v[0].pos = v[1].pos;
                    v[1].pos = v[2].pos;
                    v[2].pos = v[3].pos;
                    v[3].pos = t;
                    break;

                case M::Orientation180:
                    t = v[0].pos;
                    v[0].pos = v[2].pos;
                    v[2].pos = t;
                    
                    t = v[3].pos;
                    v[3].pos = v[1].pos;
                    v[1].pos = t;
                    break;
                    
                case M::Orientation90:
                    t = v[0].pos;
                    v[0].pos = v[3].pos;
                    v[3].pos = v[2].pos;
                    v[2].pos = v[1].pos;
                    v[1].pos = t;
                    break;
                    
                default:
                    break;
                }
            }

            auto base = pushVertices(v);

            pushIndex(base, 0, 1, 2);
            pushIndex(base, 2, 3, 0);

        }  // For tiled x
    }  // For tiled y
}  // Texture rendering

}  // namespace mangl::render
