/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#define MInlineFunc AForceInline

#define MInlineAuto MInlineFunc static auto
#define MInlineVoid MInlineFunc static void


namespace mangl::render {

struct RenderVertex {
    using T = Real;
    Point3DGL pos;
    Color color;
    PointGL texture;
    Point3DGL normal;
    Point3DGL par1;
    Point3DGL par2;
};


//
// Render pipeline global variables
//
constexpr auto pipelineColorsCount = 4;
static Color pipelineColors_[pipelineColorsCount]{};

static RenderTextureInfo *pipelineTextureInfo_{};
static SizeGL pipelineTextureSize_{};
static PointGL pipelineTextureA_{}, pipelineTextureD_{};

struct RenderContext {
    SceneP scene;
    PointGL offset;
    PrimP prim;
};

using RenderContextA = RenderContext&;

}  // namespace mangl::render

