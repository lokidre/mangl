/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_render.h"

#include "../mangl_environment.h"
#include "../mangl_scene.h"
#include "../scene/mangl_model.h"

#include <apeal/arithm.hpp>
#include <apeal/util.hpp>


#if MANGL_OPENGL
#include "../opengl/mangl_opengl.h"
#endif

#include "mangl_pl_util_header.h"
#include "mangl_pl_util_pack.h"

#include "mangl_pl_util_draw.h"


#include "mangl_pl_util_circle.h"
#include "mangl_pl_util_color.h"
#include "mangl_pl_util_depth.h"
#include "mangl_pl_util_fog.h"
#include "mangl_pl_util_light.h"
#include "mangl_pl_util_line.h"
#include "mangl_pl_util_projection.h"
#include "mangl_pl_util_rect.h"
#include "mangl_pl_util_scissor.h"
#include "mangl_pl_util_tex.h"


#include "mangl_pipeline_box.h"
#include "mangl_pipeline_line.h"
#include "mangl_pipeline_circle.h"
#include "mangl_pipeline_model.h"
#include "mangl_pipeline_point.h"
#include "mangl_pipeline_rect.h"
#include "mangl_pipeline_tex.h"
#include "mangl_pipeline_tri.h"
#include "mangl_pipeline_bezier.h"
#include "mangl_pipeline_particles.h"


//#if _DEBUG
//#define DEBUG_PIPELINE_STATE_FLUSH 1
//#endif


namespace mangl {

namespace render {

MInlineVoid pipelineRender() noexcept
{
    RenderContext ctx{};

    using T = PrimType;

    std::memset(&glState_, 0, sizeof(glState_));

    auto lastPrimType{T::None};

    for (SceneP scenePtr: *scenes_) {
        auto &scene = *scenePtr;
        if (!scene.visible_)
            continue;

        ctx.scene = scenePtr;

        for (auto &primPack: scene.objects_) {
            auto count = primPack.prim ? 1 : primPack.prims->size();
            auto primPtr = primPack.prim ? primPack.prim : primPack.prims->data();

            // auto count = primPack.first.first;  // Number of prims
            // auto stride = primPack.first.second;
            // auto primPtr = primPack.second;  // Pointer

            //for (int primIndex = 0; primIndex < count; ++primIndex, primPtr = (Prim *)(((uint8_t *)primPtr) + stride)) {
            for (int primIndex = 0; primIndex < count; ++primIndex, ++primPtr) {
                auto &prim = *primPtr;

                // Skip invisible objects
                if (!prim.visible_ || prim.type == T::None || prim.type == T::Phony)
                    continue;

                // Walk up the parents chain
                ctx.offset = {};
                bool visible = true;
                for (auto parent = prim.parent; parent; parent = parent->parent) {
                    ctx.offset.x += parent->r.x, ctx.offset.y += parent->r.y;
                    if (!parent->visible_) {
                        visible = false;
                        break;
                    }
                }
                if (!visible)
                    continue;

                // Check visibility
                switch(prim.type) {
                case T::Texture:
                case T::Rect:
                case T::Box:
                    if (prim.r.w == 0 || prim.r.h == 0) {
                        visible = false;
                        break;
                    }

                    if (prim.r.y >= cacheLayoutViewport_.y + cacheLayoutViewport_.h ||
                        prim.r.y + prim.r.h <= cacheLayoutViewport_.y) {
                        visible = false;
                        break;
                    }

                    if (prim.r.x >= cacheLayoutViewport_.x + cacheLayoutViewport_.w ||
                        prim.r.x + prim.r.w <= cacheLayoutViewport_.x) {
                        visible = false;
                        break;
                    }

                    break;

                case T::Triangle:
                    visible = false;
                    for (auto& vtx: prim.tri) {
                        if (vtx.x >= cacheLayoutViewport_.x && vtx.x < cacheLayoutViewport_.x + cacheLayoutViewport_.w) {
                            visible = true;
                            break;
                        }

                        if (vtx.y >= cacheLayoutViewport_.y && vtx.y < cacheLayoutViewport_.y + cacheLayoutViewport_.h) {
                            visible = true;
                            break;
                        }
                    }
                    break;

                default: break;
                }

                if (!visible)
                    continue;

                ctx.prim = primPtr;

                if (T::Model == prim.type) {
                    auto model = prim.model;
                    auto bbox = model->getBoundingBox();
                    auto maxf = static_cast<RealGL>(bbox.max());

                    vtxPosCurrentScale_ = maxf;
                    vtxPosScaleRecip_ = static_cast<RealGL>(1) / vtxPosCurrentScale_;

                    setUniformStateValue(&glState_.vtxPosScale, vtxPosCurrentScale_);
                } else {
                    if (lastPrimType == T::Model || lastPrimType == T::None) {
                        vtxPosCurrentScale_ = vtxPosRegularScale_;
                        vtxPosScaleRecip_ = vtxPosRegularRecip_;

                        setUniformStateValue(&glState_.vtxPosScale, vtxPosCurrentScale_);
                    }
                }

                processPipelineProjection(ctx);
                processPipelineColors(ctx);
                processScissor(ctx);
                processPrimTexture(ctx);
                processFog(ctx);
                processDepthTest(ctx);
                processShading(ctx);

                // Try to sort them by frequency
                switch (prim.type) {
                using enum PrimType;
                case Texture:    pipelineRenderTexture(ctx);     break;
                case Rect:       pipelineRenderRect(ctx);        break;
                case Box:        pipelineRenderBox(ctx);         break;
                case Line:       pipelineRenderLine(ctx);        break;
                case Triangle:   pipelineRenderTriangle(ctx);    break;
                case Model:      pipelineRenderModel(ctx);       break;
                case Circle:     pipelineRenderCircle(ctx);      break;
                case Disk:       pipelineRenderDisk(ctx);        break;
                case Point:      pipelineRenderPoint(ctx);       break;
                case Bezier:     pipelineRenderBezier(ctx);      break;

                default:
                    MDebugTrap();
                    break;
                }

                lastPrimType = prim.type;

//#if DEBUG_PIPELINE_STATE_FLUSH
                // bufferFlush();
//#endif

            }  // for prims
        }  // for prim lists

        bufferPipelineFinish();

    }  // For scenes
}

}  // namespace mangl::render


void SceneRenderer::onRender() noexcept {
    using namespace render;

    if (!onRenderMaintenance())
        return;

    if (screenshotScheduled_) {
#if MANGL_OPENGL
        opengl::RenderEngineOpenGL::screenshotPrepare();
#endif
    }

    screenScaleCache_ = screenScale_;
    cacheScreenViewport_ = screenViewport_;
    cacheLayoutViewport_ = layoutViewport_;

#if MANGL_OPENGL
    opengl::RenderEngineOpenGL::renderBegin();
#endif

    pipelineRender();
    
#if MANGL_OPENGL
    opengl::RenderEngineOpenGL::renderCommit();
#endif

    if (screenshotScheduled_) {
#if MANGL_OPENGL
        opengl::RenderEngineOpenGL::screenshotCommit();
#endif
        screenshotScheduled_ = false;
    }

}

}  // namespace mangl
