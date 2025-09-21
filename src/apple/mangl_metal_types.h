/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <simd/simd.h>

#ifdef __OBJC__
@import Metal;
#endif

namespace mangl {
namespace render {

enum PrimitiveType {
    PrimitiveTriangle = 1,
    PrimitiveLine = 2,
    PrimitivePoint = 3,
    PrimitiveFlush = 1000,
};

enum BufferIndex {
    BufferIndexVertices = 0,
    BufferIndexUniforms = 1,
};

enum TextureIndex {
    TextureIndexBaseColor = 0,
};

using BufferIndexType = uint16_t;

enum VertexAttributeIndex {
    VertexAttributeIndexPosition = 0,
    VertexAttributeIndexColor = 1,
    VertexAttributeIndexTexCoord = 2,
    VertexAttributeIndexNormal = 3,
    VertexAttributeIndexParams1 = 4,
    VertexAttributeIndexParams2 = 5,
};


using RenderVertexPosition = vector_float4;
using RenderVertexColor = vector_float4;
using RenderVertexTexture = vector_float2;
using RenderVertexNormal = vector_float2;
using RenderVertexParams1 = vector_float4;
using RenderVertexParams2 = vector_float4;

struct VertexData {
    RenderVertexPosition position;
    RenderVertexColor color;
    RenderVertexTexture texcoord;
    RenderVertexNormal normal;
    RenderVertexParams1 params1;
    RenderVertexParams2 params2;
};

#ifdef __OBJC__
constexpr auto MtlVertexFormatPosition = MTLVertexFormatFloat4;
constexpr auto MtlVertexOffsetPosition = offsetof(VertexData, position);

constexpr auto MtlVertexFormatColor = MTLVertexFormatFloat4;
constexpr auto MtlVertexOffsetColor = offsetof(VertexData, color);

constexpr auto MtlVertexFormatTexCoord = MTLVertexFormatFloat2;
constexpr auto MtlVertexOffsetTexCoord = offsetof(VertexData, texcoord);

constexpr auto MtlVertexFormatNormal = MTLVertexFormatFloat2;
constexpr auto MtlVertexOffsetNormal = offsetof(VertexData, normal);

constexpr auto MtlVertexFormatParams1 = MTLVertexFormatFloat4;
constexpr auto MtlVertexOffsetParams1 = offsetof(VertexData, params1);

constexpr auto MtlVertexFormatParams2 = MTLVertexFormatFloat2;
constexpr auto MtlVertexOffsetParams2 = offsetof(VertexData, params2);


constexpr auto MtlBufferIndexType = MTLIndexTypeUInt16;
#endif


using UniMat4 = matrix_float4x4;
using UniVec2 = vector_float2;
using UniColor = vector_float4;

using HalfFloat = __fp16;


struct Uniforms {
    int         prim_type{};  // Primitive type
    
    // Parameters scaling
    float       vtx_pos_scale{1};
    //float       vtx_nrm_scale{1};
    
    // Environment/effects
    float       screen_scale{1};
    UniVec2     viewport{};
    UniColor    scene_color_effect{};
    
    // Model/View/Projection Matrices
    UniMat4     model_mtx{};
    UniMat4     view_mtx{};
    UniMat4     proj_mtx{};
    
    // Texture
    bool        tex_enabled{};
    //UniSampler2 un_tex_sampler;

    
    // Fog
    bool        fog_enabled{};
    int fog_type{0};
    int fog_depth{0};
    float fog_density{1};
    float fog_z_near{0};
    float fog_z_far{1};
    UniColor    fog_color{};
    //InColorV    fs_fog_factor;

    
    // Line parameters
    float       line_width{};
    float       line_edge_inner{};
    float       line_edge_outer{};
    
    // Point parameters
    float       point_diameter{};
    //float       point_edge_inner{};
    //float       point_edge_outer{};

};

}  // namespace render
}  // namespace mangl

