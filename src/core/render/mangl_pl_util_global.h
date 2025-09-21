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

#include "mangl_render_vertex_pack.h"

#include "../scene/mangl_model.h"
#include "../opengl/mangl_opengl_shader.h"

#if __OBJC__
#import <Metal/Metal.h>
#endif

namespace mangl {

class SceneRenderer;

struct RenderTextureInfo {
    bool valid{};  // shows if the structure valid or not. The structures are never freed, just this flag is set to false
    int index{};  // index in the textures array

    TextureId textureId{};  // User supplied texture id or automatically generated
    String imageFile;  // the file name where the texture bitmap is store to reload if needed
    SizeI size{};  // Texture image size

    // Maintenance
    bool pendingLoad{};
    bool pendingRelease{};
    bool pendingPause{};
    bool pendingResume{};

#if MANGL_OPENGL
    // Graphics engine stuff
    GLuint glTextureId{GL_INVALID_VALUE};
    int glInternalFormat{}, glFormat{};
#endif
    
#if MANGL_METAL
#if __OBJC__
    id<MTLTexture> textureMetal{};
#else
    void* textureMetal{};
#endif
#endif

    int glTextureSlot{};


    // Graphics caching stuff
    bool glTexParametersUpdated{};  // if this texture's parameters were updated in the graphics engine

    void clear() noexcept {
        //*this = {};  // and just do like this
        valid = false;
        pendingLoad = pendingRelease = false;
#if MANGL_METAL
        textureMetal = {};
#endif
        glTexParametersUpdated = false;
    }
};


namespace render {

constexpr int MaxTextures = 8;  // total maximum textures loaded into memory simulaneously

using ScenesContainer = std::vector<SceneP>;
extern ScenesContainer* scenes_;


extern RenderTextureInfo* textures_[MaxTextures];
extern int texturesCount_;

extern SceneRenderer* sceneRender_;
}  // namespace render

namespace render {

struct ShaderProgramCont {
#if MANGL_OPENGL
    auto selectShader(opengl::ShaderPermutation pm) noexcept {
        auto it = programs.find(pm.hash);
        ADebugTrapAssert(it != programs.end());
        return it->second.get();
    }

    auto insertShader(opengl::ShaderPermutation pmu) noexcept {
        auto it = programs.insert({pmu.hash, std::make_unique<opengl::ShaderProgram>(pmu)});
        return it.first->second.get();
    }

    //std::unordered_map<unsigned, std::unique_ptr<opengl::ShaderProgram>> programs;
    std::map<unsigned, std::unique_ptr<opengl::ShaderProgram>> programs;
#endif
};

extern ShaderProgramCont programs_;

extern RealGL vtxPosRegularMaxVal_, vtxPosMaxRange_;
extern RealGL vtxPosRange_, vtxPosRegularScale_, vtxPosRegularRecip_; //, vtxPosRecip_; //inPositionScale_;

extern RealGL vtxPosCurrentMaxVal_, vtxPosCurrentScale_, vtxPosScaleRecip_;

extern RealGL screenScaleCache_;

extern RectGL cacheScreenViewport_;
extern RectGL cacheLayoutViewport_;



//extern bool defaultProjMatrixSet_, defaultViewMatrixSet_, defaultModelMatrixSet_;

constexpr auto piGL_ = cognit::constants::pi<RealGL>;

#ifndef MANGL_METAL
enum PrimitiveType {
    PrimitiveTriangle = 1,
    PrimitiveLine = 2,
    PrimitivePoint = 3,
    PrimitiveFlush = 1000,
};
#endif

//constexpr auto PrimitiveTriangle = 1;
//constexpr auto PrimitiveLine = 2;
//constexpr auto PrimitivePoint = 3;
//constexpr auto PrimitiveFlush = 1000;


using RenderIndexType = unsigned short;



using VertexIndexType = int;

struct RenderUniformsState {
    const Point3DGL* lightPos;
    const Fog* fog;
    const Material* material;
    
    const RenderTextureInfo* texture;

    const MatrixHashedGL* modelMtx, *viewMtx, *projMtx;
    RectGL scissorRect;

    RealGL lineWidth; // Also applies to point size

    RealGL vtxPosScale;

    // Storing all the booleans in the end and packing them.
    // Saved 8 bytes
    // TODO: make it bitfield
    //bool valid:1;
    //bool textureEnabled:1;
    //bool shadingEnabled:1, fogEnabled:1;
    //bool depthTest:1, cullFace:1, scissor:1;
    //bool valid;
    //bool textureEnabled;
    bool shadingEnabled;
    bool depthTest, cullFace, scissor;
};

struct DrawPrimitive {
    int batchIndex;
    int primitiveType;
    int vtxStart, vtxCount;
    int idxStart, idxCount;
    RenderUniformsState state;  // throw the state in the back to avoid padding
};

struct DrawBatch {
    int vtxStart{}, vtxCount{};
    PackedRenderVertex* vtxData{};

    int idxStart{}, idxCount{};
    RenderIndexType *idxData{};

    int cmdStart{}, cmdCount{};
    DrawPrimitive *cmdData{};
};


struct PipelineStatics {
    int primitiveType;

    int commandHead;
    int currBatchIndex, batchHead;

    //PackedRenderVertex *vertexData;
    int vertexStart, vertexHead;
    int batchVtxStart, batchIdxStart;

    //RenderIndexType *indexData;
    int indexStart, indexHead;


    DrawBatch *currBatch;

    TextureId lastTextureId[2];
    RenderTextureInfo* lastTextureInfo[2];
    int lastTextureIndex;
};

extern PipelineStatics pipelineStatics_;

extern RenderUniformsState glState_;

extern int maxRenderBufferSize_;
extern int maxBatchVtxCount_, maxBatchIdxCount_;


extern apl::Buffer<DrawBatch> drawBatches_;

extern apl::Buffer<PackedRenderVertex> vertexBuffer_;
extern apl::Buffer<RenderIndexType> indexBuffer_;

extern apl::Buffer<DrawPrimitive> drawCommands_;

extern PackedRenderVertex *vertexData_;
// extern int vertexStart_, vertexHead_;
// extern int batchVtxStart_, batchIdxStart_;


extern RenderIndexType *indexData_;
// extern int indexStart_, indexHead_;



inline void render_statics_init() noexcept {
    pipelineStatics_ = {};
    // Triggers new batch allocation
    //commandHead_ = 0;

    //batchHead_ = {};
    //currBatchIndex_ = 0;
    //batchHead_ = 0;
    //currBatch_ = nullptr;

    //vertexStart_ = vertexHead_ = 0;
    //indexStart_ = indexHead_ = 0;

    //batchVtxStart_ = batchIdxStart_ = 0;


    //pipelineStatics_.primitiveType = {};
}

inline void render_statics_reset() noexcept {
    render_statics_init();
}

//}  // namespace render_statics


}  // namespace render

} // namespace mangl
