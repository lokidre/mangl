/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_setup.h"

#if MANGL_OPENGL
#include "../opengl/mangl_opengl.h"
#endif

#if MANGL_METAL
#include "../../apple/mangl_metal_types.h"
#endif


namespace mangl::render {

#if MANGL_OPENGL
using IndexBufferType = GLushort;

// Packing data is important. The data is sent to graphics engine
//#pragma pack(push, 1)

//using RenderPositionCoord = GLfloat;
//using RenderColorComponent = GLfloat;
//using RenderTextureCoord = GLfloat;

using RenderPositionCoord = GLshort;
using RenderColorComponent = GLubyte;
using RenderTextureCoord = GLushort;

#define MANGL_GL_ALIGN alignas(4)

struct MANGL_GL_ALIGN RenderVertexPosition {
    using T = RenderPositionCoord;
    T x, y, z, w;
};

struct MANGL_GL_ALIGN RenderVertexColor {
    using T = RenderColorComponent;
    T r, g, b, a;
};

struct MANGL_GL_ALIGN RenderVertexTexture {
    using T = RenderTextureCoord;
    T x, y;
};

using RenderNormalCoord = RenderPositionCoord;


// MARK: Normals
struct MANGL_GL_ALIGN RenderVertexNormal {
    using T = RenderNormalCoord;
    T x, y;
};

struct MANGL_GL_ALIGN RenderVertexParams1 {
    using T = RenderPositionCoord;
    T a1, a2, a3, a4;
};

struct MANGL_GL_ALIGN RenderVertexParams2 {
    using T = RenderPositionCoord;
    T a1, a2;
};



#elif MANGL_METAL

#define MANGL_GL_ALIGN

using IndexBufferType = unsigned short;
using RenderPositionCoord = float;
using RenderColorComponent = float;
using RenderTextureCoord = float;


using RenderNormalCoord = RenderPositionCoord;

#endif

constexpr auto SizeOfIndexBufferIndex = sizeof(IndexBufferType);


constexpr auto VertexPositionComponents = 4;
constexpr auto PositionPackSize = sizeof(RenderVertexPosition);

constexpr auto VertexColorComponents = 4;
constexpr auto ColorPackSize = sizeof(RenderVertexColor);

constexpr auto VertexTextureComponents = 2;
constexpr auto TexturePackSize = sizeof(RenderVertexTexture);


//
// Normals


constexpr auto VertexNormalComponents = 2;
constexpr auto NormalPackSize = sizeof(RenderVertexNormal);


constexpr auto VertexParams1Components = 4;
constexpr auto VertexParams2Components = 2;

//constexpr auto VertexParamsComponents = 6;
constexpr auto SizeOfParamsPack = sizeof(RenderVertexParams1) + sizeof(RenderVertexParams2);

// Make it 32 bytes packed for OpenGL compact
struct MANGL_GL_ALIGN PackedRenderVertex {
    RenderVertexPosition position;  // 8 bytes: 4 words per position (w contains normal z)
    RenderVertexColor color;  // 4 bytes: 1 byte per color component
    RenderVertexTexture texture;  // 4 bytes: 2 coordinates, 1 word per coordinates
    RenderVertexNormal normal;  // 4 bytes: 2 coordinates, 1 word per each
    RenderVertexParams1 params1; // 12 bytes:
    RenderVertexParams2 params2; //
};
constexpr auto SizeOfVertexPack = sizeof(PackedRenderVertex);


// Check for padding
static_assert(SizeOfVertexPack == PositionPackSize + ColorPackSize + TexturePackSize + NormalPackSize + SizeOfParamsPack);



//template <int S>
//struct Msg;
//Msg<SizeOfVertexPack> aa;
//#pragma pack(pop)

}  // namespace mangl::render

