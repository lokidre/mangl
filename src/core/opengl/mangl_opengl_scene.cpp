/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_render.h"

#if MANGL_OPENGL

#include "../mangl_config.h"

#include "mangl_opengl.h"

#include "../render/mangl_render_vertex_pack.h"
#include "../render/mangl_pl_util_global.h"
#include "../render/mangl_pl_util_header.h"



namespace mangl {

void SceneRenderer::prepareSceneOpenGL() {
    using namespace opengl;
    using namespace render;

    if (scenePrepared_)
        return;
    scenePrepared_ = true;

    if (ManglConfig::debugPrintGraphics) {
        MDebugFunc();
    }



    auto glVersion = (const char *)glGetString(GL_VERSION);
    auto glslVersion = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);

    apl::verify(glVersion && glslVersion, "Failed to get glVersion");

    auto readVersion = [this](const char* verString) -> int {
        if (!verString)
            return 0;

        auto digit = verString;
        while (*digit && (*digit < '0' || *digit > '9'))
            ++digit;

        int verHi = 1, verLo = 0;
        checked_sscanf(2, digit, "%d.%d", &verHi, &verLo);
        if (verLo < 10)
            verLo *= 10;

        return verHi * 100 + verLo;
    };

    supportedOpengGL = readVersion(glVersion);
    supportedGLSL = readVersion(glslVersion);
    isGLSLES = strstr(glslVersion, " ES") || strstr(glslVersion, " es");

    //if(supportedGLSL < 300) {
    //maxRenderBufferSize_ = 15000;
    maxRenderBufferSize_ = 16000;
    //maxRenderBufferSize_ = 800;
    //maxRenderBufferSize_ = 500000;
    //}

    maxBatchVtxCount_ = maxRenderBufferSize_ / SizeOfVertexPack;
    maxBatchIdxCount_ = maxRenderBufferSize_ / SizeOfIndexBufferIndex;
    render_statics_init();


    if (ManglConfig::debugPrintGraphics) {
        MDebugPrint("OpenGL Version: %d : %s", supportedOpengGL, glVersion);
        MDebugPrint("Shader Version: %d : %s", supportedGLSL, glslVersion);
    }

    loadShaders();
    prepareViewport();


    //
    // Create the buffers
    //

    glGenVertexArrays(1, &RenderEngineOpenGL::glVertexArray); ManglAssertOpenGL();
    glBindVertexArray(RenderEngineOpenGL::glVertexArray); ManglAssertOpenGL();

    glGenBuffers(1, &RenderEngineOpenGL::glVertexBufferObject);  ManglAssertOpenGL();
    glBindBuffer(GL_ARRAY_BUFFER, RenderEngineOpenGL::glVertexBufferObject);  ManglAssertOpenGL();

    //
    // Index Buffer
    //
    glGenBuffers(1, &RenderEngineOpenGL::glIndexBufferObject); ManglAssertOpenGL();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderEngineOpenGL::glIndexBufferObject); ManglAssertOpenGL();


    // glVertexAttribPointer parameters
    GLuint slot{};
    GLint components{};
    GLenum type{};
    GLboolean normalized{};
    GLsizei stride = SizeOfVertexPack;
    void* offset{};



    //
    // Position
    //
    slot = ShaderProgram::VtxPosSlot;
    offset = reinterpret_cast<void*>(offsetof(PackedRenderVertex, position));
    components = VertexPositionComponents; //4;
    if constexpr (std::is_floating_point_v<RenderPositionCoord>) {
        type = GL_FLOAT;
        normalized = GL_FALSE;
    } else {
        type = GL_SHORT;
        normalized = GL_TRUE;
    }

    glVertexAttribPointer(slot, components, type, normalized, stride, offset);  ManglAssertOpenGL();
    glEnableVertexAttribArray(slot);
    RenderEngineOpenGL::cacheVtxPosEnabled_ = true;
    //shader->vtx_pos.enable();


    //
    // Color
    //
    slot = ShaderProgram::VtxClrSlot;
    offset = reinterpret_cast<void*>(offsetof(PackedRenderVertex, color));
    components = VertexColorComponents;  // 4
    if constexpr (std::is_floating_point_v<RenderColorComponent>) {
        type = GL_FLOAT ;
        normalized = GL_FALSE;
    } else {
        type = GL_UNSIGNED_BYTE;
        normalized = GL_TRUE;
    }

    glVertexAttribPointer(slot, components, type, normalized, stride, offset);  ManglAssertOpenGL();
    glEnableVertexAttribArray(slot);
    RenderEngineOpenGL::cacheVtxClrEnabled_ = true;


    //
    // Texture coordinates
    //
    slot = 2; //shader->vtx_tex.slot;
    offset = reinterpret_cast<void*>(offsetof(PackedRenderVertex, texture));
    components = VertexTextureComponents;
    if constexpr (std::is_floating_point_v<RenderTextureCoord>) {
        type = GL_FLOAT ;
        normalized = GL_FALSE;
    } else {
        type = GL_UNSIGNED_SHORT;
        normalized = GL_TRUE;
    }

    glVertexAttribPointer(slot, components, type, normalized, stride, offset);  ManglAssertOpenGL();
    glDisableVertexAttribArray(slot);
    RenderEngineOpenGL::cacheVtxClrEnabled_ = false;

    //
    // Normals
    //
    slot = ShaderProgram::VtxNrmSlot;
    offset = reinterpret_cast<void*>(offsetof(PackedRenderVertex, normal));
    components = VertexNormalComponents;
    if constexpr (std::is_floating_point_v<RenderNormalCoord>) {
        type = GL_FLOAT;
        normalized = GL_FALSE;
    } else {
        type = GL_SHORT;
        normalized = GL_TRUE;
    }

    glVertexAttribPointer(slot, components, type, normalized, stride, offset);   ManglAssertOpenGL();
    glDisableVertexAttribArray(slot); ManglAssertOpenGL();
    RenderEngineOpenGL::cacheVtxNrmEnabled_ = false;


    for (auto& shaderIt: programs_.programs) {
        RenderEngineOpenGL::cacheShader_ = shaderIt.second.get();
        glUseProgram(RenderEngineOpenGL::cacheShader_->handle);
        RenderEngineOpenGL::oglUpdateLineWidth(1);
    }
    RenderEngineOpenGL::cacheShader_ = nullptr;


    RenderEngineOpenGL::cacheCullEnabled_ = false;
    glDisable(GL_CULL_FACE); ManglAssertOpenGL();
    //glFrontFace(GL_CCW); ManglAssertOpenGL();
    glFrontFace(GL_CW); ManglAssertOpenGL();

    glCullFace(GL_BACK); ManglAssertOpenGL();


    glDisable(GL_DITHER); ManglAssertOpenGL();

    // #ifdef GL_MULTISAMPLE
    //     glDisable(GL_MULTISAMPLE); ManglAssertOpenGL();
    // #endif


    // Clears the view with black
    if (!transparent) {
        glClearColor(0, 0, 0, 1); ManglAssertOpenGL();
    } else {
        glClearColor(0, 0, 0, 0);  ManglAssertOpenGL();
    }

    // Depth test
    glEnable(GL_DEPTH_TEST); ManglAssertOpenGL();
    //#if MANGL_OPENGL_ES

    // clang-format off
#ifndef _MSC_VER
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-pointer-compare"
#endif
    // NOLINTNEXTLINE(clang-analyzer-core.CallAndMessage)
    if (glDepthRangef != nullptr) {
        glDepthRangef(0, 1); ManglAssertOpenGL();
    } else {
        //glDepthRange(0, 1); ManglAssertOpenGL();
    }
#ifndef _MSC_VER
#pragma clang diagnostic pop
#endif
    // clang-format on

    // #endif
    RenderEngineOpenGL::cacheDepthTestEnabled_ = false;
    glDepthFunc(GL_LESS); ManglAssertOpenGL();
    //glDepthFunc(GL_ALWAYS); ManglAssertOpenGL();
    glDisable(GL_DEPTH_TEST); ManglAssertOpenGL();

    //
    // Point primitives
    //
    // For IOS point sprites are always enabled

#if APEAL_OS_WINDOWS  // For Windows we have to enable them
    glEnable(GL_POINT_SPRITE); ManglAssertOpenGL();
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE); ManglAssertOpenGL();
#endif

#if APEAL_OS_ANDROID  // Android TODO:
    //glEnable(GL_POINT_SPRITE_OES);  ManglAssertOpenGL() ;
    //glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_OES) ;  ManglAssertOpenGL() ;
#endif


    glDepthFunc(GL_LEQUAL); ManglAssertOpenGL();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); ManglAssertOpenGL();
    glEnable(GL_BLEND); ManglAssertOpenGL();
    //glDisable(GL_BLEND); ManglAssertOpenGL();
    glDisable(GL_SCISSOR_TEST); ManglAssertOpenGL();

    //cache_.bound2DTexture = (GLuint)-1;

    //drawFence = glFenceSyncAPPLE(GL_SYNC_GPU_COMMANDS_COMPLETE_APPLE, 0) ;

}

void SceneRenderer::destroySceneOpenGL() {
    MDebugFunc();

    scenePrepared_ = false;

    using namespace opengl;

    destroyShadersOpenGL();

    glDeleteVertexArrays(1, &RenderEngineOpenGL::glVertexArray); ManglAssertOpenGL();
    glDeleteBuffers(1, &RenderEngineOpenGL::glVertexBufferObject);  ManglAssertOpenGL();
    glDeleteBuffers(1, &RenderEngineOpenGL::glIndexBufferObject); ManglAssertOpenGL();

}

} // namespace mangl

#endif

