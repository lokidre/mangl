/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_pl_util_header.h"
#include "mangl_pl_util_global.h"
#include "mangl_render_vertex_pack.h"

// #if __cpp_concepts
// #include <concepts>
// #endif


namespace mangl::render {

//
// MARK: Position packing
//

// Floating point position
template <class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
MInlineAuto packPosition(PackedRenderVertex& v, apl::Point3D<T> p) noexcept {
    const auto& s = vtxPosScaleRecip_;

    v.position = {p.x * s, p.y * s, p.z * s};
}

// Packed integer position
template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
MInlineAuto packPosition(PackedRenderVertex& v, Point3DGL p) noexcept {
    const auto& s = vtxPosScaleRecip_;
    const auto& t = vtxPosCurrentMaxVal_;

    v.position.x = static_cast<T>(p.x * s * t);
    v.position.y = static_cast<T>(p.y * s * t);
    v.position.z = static_cast<T>(p.z * s * t);
    //v.position.x = (T)(p.x * s);
    //v.position.y = (T)(p.y * s);
    //v.position.z = (T)(p.z * s);
}



//
// Normal packing
// Normal uses position's 4th coordinate for z coordinate
//

// Floating point normal
template <class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
MInlineAuto packNormal(PackedRenderVertex &v, Point3DGL n) noexcept {
    v.normal.x = n.x;
    v.normal.y = n.y;
    //v.normal.z = (T)n.z;
    v.position.w = n.z;
}

// Integer packing
template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
MInlineAuto packNormal(PackedRenderVertex &v, Point3DGL n) noexcept {
    //using std::clamp;

    const auto& s = vtxPosCurrentMaxVal_;

    v.normal.x = static_cast<T>(n.x * s);
    v.normal.y = static_cast<T>(n.y * s);
    v.position.w = static_cast<T>(n.z * s);

    //v.normal.x = n.x;
    //v.normal.y = n.y;
    //v.position.w = n.z;
}




//
// MARK: Texture packing
//

// Floating point
// GLfloat
// #if __cpp_concepts
// template <std::floating_point T>
// #else
template <class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
//#endif
MInlineAuto packTexture(PackedRenderVertex &v, PointGL t) noexcept {
    v.texture.x = t.x / pipelineTextureSize_.w;
    v.texture.y = t.y / pipelineTextureSize_.h;
}

// Integer packing
// GLushort
//template <std::integral T>
template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
MInlineAuto packTexture(PackedRenderVertex &v, PointGL t) noexcept {
    constexpr auto mx = (RealGL)std::numeric_limits<T>::max();
    constexpr auto rng = mx;

    v.texture.x = (T)(t.x / pipelineTextureSize_.w * rng + 1);
    v.texture.y = (T)(t.y / pipelineTextureSize_.h * rng + 1);
}





//
// Color packing
//

// GLfloat
template <class T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
MInlineAuto packColor(PackedRenderVertex &v, ColorA c) noexcept {
    constexpr T mn{0}, mx{1};
    using std::clamp;

    v.color.r = clamp(c.r, mn, mx);
    v.color.g = clamp(c.g, mn, mx);
    v.color.b = clamp(c.b, mn, mx);
    v.color.a = clamp(c.a, mn, mx);
}

// GLubyte
template <class T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
MInlineAuto packColor(PackedRenderVertex &v, ColorA c) noexcept {
    auto clamp = [](RealGL x) -> T {
        return (T)(x <= 0.f ? 0.f : x >= 255.f ? 255.f : x); 
    };

    v.color.r = clamp(c.r * 255);
    v.color.g = clamp(c.g * 255);
    v.color.b = clamp(c.b * 255);
    v.color.a = clamp(c.a * 255);
}


MInlineAuto packVertex(const RenderVertex &v) noexcept {
    PackedRenderVertex p{};
    packPosition<RenderPositionCoord>(p, v.pos);
    packColor<RenderColorComponent>(p, v.color);

    if (glState_.texture)
        packTexture<RenderTextureCoord>(p, v.texture);

    if (glState_.shadingEnabled)
        packNormal<RenderNormalCoord>(p, v.normal);

    return p;
}



}  // namespace mangl::opengl

