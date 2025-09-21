/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_pl_util_global.h"

#include "../mangl_environment.h"
#include "../mangl_scene.h"

namespace mangl::render {

ScenesContainer *scenes_{};

RenderTextureInfo *textures_[MaxTextures];
int texturesCount_{};

SceneRenderer *sceneRender_{};

ShaderProgramCont programs_;

//RealGL inPositionScale_{};
RealGL vtxPosMaxVal_, vtxPosMaxRange_, vtxPosRegularMaxVal_;
RealGL vtxPosRange_, vtxPosRegularScale_, vtxPosRegularRecip_;

RealGL vtxPosCurrentMaxVal_, vtxPosCurrentScale_, vtxPosScaleRecip_;


RealGL screenScaleCache_{1};

RectGL cacheScreenViewport_{};
RectGL cacheLayoutViewport_{};


RenderUniformsState glState_{};

PipelineStatics pipelineStatics_{};


//int primitiveType_{};

int maxRenderBufferSize_{};
int maxBatchVtxCount_{}, maxBatchIdxCount_{};

//int currBatchIndex_{}, batchHead_{};
apl::Buffer<DrawBatch> drawBatches_{};
//DrawBatch *currBatch_{};

apl::Buffer<PackedRenderVertex> vertexBuffer_;
apl::Buffer<RenderIndexType> indexBuffer_;

//int commandHead_;
apl::Buffer<DrawPrimitive> drawCommands_;


PackedRenderVertex *vertexData_{};
// int vertexStart_{}, vertexHead_{};
// int batchVtxStart_{}, batchIdxStart_{};
//
RenderIndexType *indexData_{};
// int indexStart_{}, indexHead_{};

}   // namespace mangl::render
