/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_render.h"
#include "../mangl_common.h"
#include "../mangl_config.h"

#include "../mangl_environment.h"


#include "../render/mangl_render_vertex_pack.h"
#include "../render/mangl_pl_util_global.h"
#include "../render/mangl_pl_util_header.h"

#include "mangl_opengl.h"


#if MANGL_OPENGL

#include "mangl_opengl_shader.h"


// TODO: figure out how to check the actual values of the model matrices

#define MANGL_OPENGL_SHADERS_SPLIT  1

//#define MANGL_OPENGL_ORPHANED_BUFFERS 1
#define MANGL_OPENGL_RECREATE_BUFFERS 1


namespace mangl::opengl {
using namespace mangl::render;

//static ShaderPermutation cachePermutation_{};
//static ShaderProgram* cacheShader_{};
//
//static GLfloat cacheScreenScale_{};
//static GLfloat cacheLineWidth_{}, cachePointSize_{};
//
//static bool cacheVtxPosEnabled_{}, cacheVtxClrEnabled_{};
//static bool cacheVtxTexEnabled_{}, cacheVtxNrmEnabled_{};
//
//[[maybe_unused]] static int maxVertexBufferCount_{}, maxIndexBufferCount_{};
//
//static bool cacheCullEnabled_{}, cacheDepthTestEnabled_{};
//
//static bool cacheScissorEnabled_{};
//static RectGL cacheScissorRect_{};


void RenderEngineOpenGL::oglUpdateLineWidth(GLfloat width) {
    cacheLineWidth_ = width;

    auto scaled = width * cacheScreenScale_;

    auto outerEdge = scaled;
    auto innerEdge = outerEdge / 4;

#if 0
    cacheShader_->line_width = scaled;
    cacheShader_->line_edge_inner = innerEdge;
    cacheShader_->line_edge_outer = outerEdge;
#else
    cacheShader_->line_params.set({scaled, innerEdge, outerEdge});
#endif

    glLineWidth(scaled * 1.6f);
}

void RenderEngineOpenGL::oglUpdatePointSize(GLfloat size) {
    cachePointSize_ = size;

    auto scaled = size * cacheScreenScale_;

    auto outerEdge = scaled;
    auto innerEdge = outerEdge / 4;

#if 0
    cacheShader_->line_width = scaled;
    cacheShader_->line_edge_inner = innerEdge;
    cacheShader_->line_edge_outer = outerEdge;
#else
    cacheShader_->line_params.set({scaled, innerEdge, outerEdge});
#endif

#ifndef MANGL_OPENGL_ES
    glPointSize(scaled * 1.8f);
#endif
}


MInlineAuto sendOpenGLBuffersData(const DrawBatch *batch) noexcept {
    auto vtxData = batch->vtxData;
    auto vtxSize = batch->vtxCount * SizeOfVertexPack;

    auto idxData = batch->idxData;
    auto idxSize = batch->idxCount * SizeOfIndexBufferIndex;


#if MANGL_OPENGL_ORPHANED_BUFFERS

    // Keep allocating the buffer of the same size over and over
    if (batch->vtxCount > maxVertexBufferCount_)
        maxVertexBufferCount_ = batch->vtxCount;
    glBufferData(GL_ARRAY_BUFFER, maxVertexBufferCount_ * SizeOfVertexPack, 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vtxSize, vtxData);

    if (batch->idxCount > maxIndexBufferCount_)
        maxIndexBufferCount_ = batch->idxCount;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxIndexBufferCount_ * SizeOfIndexBufferIndex, 0, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, idxSize, idxData);

#elif MANGL_OPENGL_RECREATE_BUFFERS

    glBufferData(GL_ARRAY_BUFFER, vtxSize, vtxData, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, idxData, GL_DYNAMIC_DRAW);

#else

    // Check the vertex buffer
    // Observation: this is the slowest on OpenGL ES 2.0/iPad Mini
    if (batch->vtxCount > maxVertexBufferCount_) {
        glBufferData(GL_ARRAY_BUFFER, vtxSize, vtxData, GL_DYNAMIC_DRAW);
        maxVertexBufferCount_ = batch->vtxCount;
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, vtxSize, vtxData);
    }

    // Same with the index buffer
    if (batch->idxCount > maxIndexBufferCount_) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, idxData, GL_DYNAMIC_DRAW);
        maxIndexBufferCount_ = batch->idxCount;
    } else {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, idxSize, idxData);
    }
#endif
}


void RenderEngineOpenGL::renderBegin() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); ManglAssertOpenGL();
}


MInlineAuto renderCommand(DrawPrimitive *cmd) noexcept
{
    constexpr GLushort* idxBaseZero{};
    const auto& state = cmd->state;


    ShaderPermutation pmu{};

    GLenum glElem{};

    switch(cmd->primitiveType) {
    case PrimitiveTriangle:
        glElem = GL_TRIANGLES;
        pmu.bits.tri = 1;
        break;

    case PrimitiveLine:
        glElem = GL_LINES;
        //if (cacheLineWidth_ != state.lineWidth)
        //    oglUpdateLineWidth(state.lineWidth);
        pmu.bits.line = 1;
        break;

    case PrimitivePoint:
        glElem = GL_POINTS;
        //if (cachePointSize_ != state.lineWidth)
        //    oglUpdatePointSize(state.lineWidth);
        pmu.bits.point = 1;
        break;

    default:
        MDebugTrap();
        break;
    }

    pmu.bits.tex = state.texture ? 1: 0; //Enabled;
    pmu.bits.fog = state.fog ? 1 : 0; //Enabled;
    pmu.bits.shading = state.shadingEnabled;

    if (!RenderEngineOpenGL::cacheShader_ || RenderEngineOpenGL::cachePermutation_.hash != pmu.hash) {
        RenderEngineOpenGL::cacheShader_ = programs_.selectShader(pmu);
        //RenderEngineOpenGL::cacheShader_ = programs_.selectShader({.hash = 0xFFFF});
        RenderEngineOpenGL::cachePermutation_ = pmu;

        glUseProgram(RenderEngineOpenGL::cacheShader_->handle); ManglAssertOpenGL();
    }


    switch(cmd->primitiveType) {
    case PrimitiveLine:
        if (RenderEngineOpenGL::cacheLineWidth_ != state.lineWidth)
            RenderEngineOpenGL::oglUpdateLineWidth(state.lineWidth);
        break;

    case PrimitivePoint:
        if (RenderEngineOpenGL::cachePointSize_ != state.lineWidth)
            RenderEngineOpenGL::oglUpdatePointSize(state.lineWidth);
        break;

    default:
        break;
    }


    auto shader = RenderEngineOpenGL::cacheShader_;

#if !MANGL_OPENGL_SHADERS_SPLIT
    shader->fog_enabled = state.fogEnabled;
    shader->prim_type = cmd->primitiveType;
    shader->tex_enabled = state.textureEnabled;
#endif

    shader->vtx_pos_scale = state.vtxPosScale;
    //cacheShader_->vtx_nrm_scale = state.vtxNrmScale;


    if (state.texture) {
        shader->tex_sampler = state.texture->glTextureSlot;
        if (!RenderEngineOpenGL::cacheVtxTexEnabled_) {
            glEnableVertexAttribArray(ShaderProgram::VtxTexSlot);
            RenderEngineOpenGL::cacheVtxTexEnabled_ = true;
        }
    } else {
        if (RenderEngineOpenGL::cacheVtxTexEnabled_) {
            glDisableVertexAttribArray(ShaderProgram::VtxTexSlot);
            RenderEngineOpenGL::cacheVtxTexEnabled_ = false;
        }
    }


    // Various Open GL state stuff
    if (state.cullFace != RenderEngineOpenGL::cacheCullEnabled_) {
        RenderEngineOpenGL::cacheCullEnabled_ = state.cullFace;
        RenderEngineOpenGL::cacheCullEnabled_ ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
    }

    if (state.depthTest != RenderEngineOpenGL::cacheDepthTestEnabled_) {
        RenderEngineOpenGL::cacheDepthTestEnabled_ = state.depthTest;
        RenderEngineOpenGL::cacheDepthTestEnabled_ ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
    }


    auto updateScissor{false};
    if (state.scissor) {
        if (!RenderEngineOpenGL::cacheScissorEnabled_) {
            updateScissor = true;
        } else {
            if (RenderEngineOpenGL::cacheScissorRect_ != state.scissorRect)
                updateScissor = true;
        }
    } else {
        if (state.scissor != RenderEngineOpenGL::cacheScissorEnabled_)
            updateScissor = true;
    }

    if (updateScissor) {
        RenderEngineOpenGL::cacheScissorEnabled_ = state.scissor;
        RenderEngineOpenGL::cacheScissorRect_ = state.scissorRect;

        if (state.scissor) {
            auto a2 = state.scissorRect.pos();
            auto c2 = a2 + state.scissorRect.size().point();

            Point3DGL a{a2.x, a2.y, 1}, c{c2.x, c2.y, 1};

            //a = *state.projMtx * (*state.viewMtx * (*state.modelMtx * a));
            a = *state.viewMtx * (*state.modelMtx * a);
            a *= RenderEngineOpenGL::cacheScreenScale_;


            c = *state.viewMtx * (*state.modelMtx * c);
            c *= RenderEngineOpenGL::cacheScreenScale_;


            glEnable(GL_SCISSOR_TEST);

            auto scx = static_cast<GLint>(a.x);
            auto scy = static_cast<GLint>(a.y);
            auto scw = static_cast<GLsizei>(c.x - a.x);
            auto sch = static_cast<GLsizei>(c.y - a.y);
            glScissor(scx, scy, scw, sch);
        } else {
            glDisable(GL_SCISSOR_TEST);
        }
    }



    if (state.fog) {
        auto fog = state.fog;

        auto fogType = static_cast<int>(fog->type);
        auto fogDepth = static_cast<int>(fog->depth);

#if MANGL_OPENGL_FOG_UNI_PACKED
        shader->fog_density = fog->density;
        shader->fog_near = fog->znear;
        shader->fog_far = fog->zfar;
        shader->fog_type = fogType;
        shader->fog_depth = fogDepth;
#else
        shader->fog_iparams.set({fogType, fogDepth});
        shader->fog_params.set({fog->density, fog->znear, fog->zfar});
#endif
        shader->fog_color = fog->color;
    }

    if (state.shadingEnabled) {
        shader->light_pos = *state.lightPos;
        shader->diff_mtl = state.material->diffuse;
        shader->spec_mtl = state.material->specular;
        shader->shininess = state.material->shininess;

        if (!RenderEngineOpenGL::cacheVtxNrmEnabled_) {
            glEnableVertexAttribArray(ShaderProgram::VtxNrmSlot);
            RenderEngineOpenGL::cacheVtxNrmEnabled_ = true;
        }
    } else {
        if (RenderEngineOpenGL::cacheVtxNrmEnabled_) {
            glDisableVertexAttribArray(ShaderProgram::VtxNrmSlot);
            RenderEngineOpenGL::cacheVtxNrmEnabled_ = false;
        }
    }


    if (shader->model_mtx.hash != state.modelMtx->hash) {
        shader->model_mtx = state.modelMtx;
    }

    if (shader->view_mtx.hash != state.viewMtx->hash) {
        shader->view_mtx = state.viewMtx;
    }

    if (shader->proj_mtx.hash != state.projMtx->hash) {
        shader->proj_mtx = state.projMtx;
    }

    glDrawElements(glElem, cmd->idxCount, GL_UNSIGNED_SHORT, idxBaseZero + cmd->idxStart);
}


void RenderEngineOpenGL::renderCommit() {
    //if (!cacheShader_)
    //    return;

    //glClear(GL_DEPTH_BUFFER_BIT);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto&& batchBegin = drawBatches_.data();
    auto&& batchEnd = batchBegin + pipelineStatics_.batchHead;

    for (auto batch = batchBegin; batch != batchEnd; ++batch) {

        sendOpenGLBuffersData(batch);

        auto&& cmdBegin = batch->cmdData;
        auto&& cmdEnd = cmdBegin + batch->cmdCount;

        for (auto cmd = cmdBegin; cmd != cmdEnd; ++cmd) {
            renderCommand(cmd);
        }  // for all commands

    }  // for all batches

    // Clear everything once we are done
    render_statics_reset();
}



void manglAssertOpenGlBody(const char* file, int line) {

#if MANGL_OPENGL_ES

    for (;;) {
        GLenum ec = glGetError();
        if (GL_NO_ERROR == ec)
            break;
        char const* s;
        switch (ec) {
        case GL_INVALID_ENUM: s = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE:  s = "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:  s = "GL_INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY:  s = "GL_OUT_OF_MEMORY"; break;
        default:  s = "UNKNOWN"; break;
        }
        MDebugFunc("%s:%d: %0x %s", file, line, (unsigned)ec, s);
    }

#else // Plain OpenGL

    for (;;) {
        GLenum ec = glGetError();
        if (GL_NO_ERROR == ec)
            break;

        auto s = (const char*)gluErrorString(ec);
        MDebugFunc("%s:%d: %0X %s", file, line, (unsigned)ec, s);
    }

#endif
}

}  // namespace mangl::opengl


namespace mangl {


bool SceneRenderer::checked_sscanf(int count, const char *buf, const char *fmt, ...) {
    std::va_list ap{};
    va_start(ap, fmt);

    if (std::vsscanf(buf, fmt, ap) != count)
        return false;

    va_end(ap);
    return true;
}





}  // namespace mangl

#endif // OpenGL render

