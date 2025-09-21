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
#include "mangl_pl_util_buffers.h"
#include "mangl_pl_util_pack.h"

namespace mangl::render {

MInlineAuto vertexBase() noexcept { return pipelineStatics_.vertexHead; }

MInlineAuto pushVertex(const RenderVertex &v) noexcept {
    auto index = pipelineStatics_.vertexHead;
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(v);
    return index - pipelineStatics_.batchVtxStart;
}

MInlineAuto pushVertices(std::span<RenderVertex> v) noexcept
{
    auto index = pipelineStatics_.vertexHead;

    for (auto &i: v)
        vertexData_[pipelineStatics_.vertexHead++] = packVertex(i);

    return index - pipelineStatics_.batchVtxStart;
}


MInlineAuto pushVertices(std::initializer_list<RenderVertex> v) noexcept
{
    auto index = pipelineStatics_.vertexHead;

    for (auto &i: v)
        vertexData_[pipelineStatics_.vertexHead++] = packVertex(i);

    return index - pipelineStatics_.batchVtxStart;
}


//// TODO: variable args
//template <class ...Pack>
//MInlineAuto pushVertices(const Pack&... v) noexcept 
//{
//    auto index = vertexHead_;
//    ((vertexData_[vertexHead_++] = packVertex(v)), ...);
//    //((indexData_[indexHead_++] = idx), ...);
//
//    //GLContext::vertexData[GLContext::vertexHead++] = packVertex(v);
//    return index - batchVtxStart_;
//}


MInlineAuto pushVertices(const RenderVertex &u, const RenderVertex &v) noexcept {
    auto index = pipelineStatics_.vertexHead;
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(u);
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(v);
    return index - pipelineStatics_.batchVtxStart;
}

MInlineAuto pushVertices(const RenderVertex &u, const RenderVertex &v, const RenderVertex &w) noexcept 
{
    auto index = pipelineStatics_.vertexHead;
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(u);
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(v);
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(w);
    return index - pipelineStatics_.batchVtxStart;
}

MInlineAuto pushVertices(const RenderVertex &u, const RenderVertex &v, const RenderVertex &w, const RenderVertex &z) noexcept 
{
    auto index = pipelineStatics_.vertexHead;
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(u);
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(v);
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(w);
    vertexData_[pipelineStatics_.vertexHead++] = packVertex(z);
    return index - pipelineStatics_.batchVtxStart;
}


// TODO: varargs

MInlineAuto pushIndex(int base, int index) noexcept {
    indexData_[pipelineStatics_.indexHead++] = static_cast<RenderIndexType>(base + index);
}


MInlineAuto pushIndex(int base, int a, int b) noexcept {
    indexData_[pipelineStatics_.indexHead++] = static_cast<RenderIndexType>(base + a);
    indexData_[pipelineStatics_.indexHead++] = static_cast<RenderIndexType>(base + b);
}

MInlineAuto pushIndex(int base, int a, int b, int c) noexcept {
    indexData_[pipelineStatics_.indexHead++] = static_cast<RenderIndexType>(base + a);
    indexData_[pipelineStatics_.indexHead++] = static_cast<RenderIndexType>(base + b);
    indexData_[pipelineStatics_.indexHead++] = static_cast<RenderIndexType>(base + c);
}

template <typename ...Pack>
MInlineAuto pushIndices(int base, Pack... idx) noexcept {
    ((indexData_[pipelineStatics_.indexHead++] = static_cast<RenderIndexType>(base + idx)), ...);
}


MInlineAuto beginTriangles(int count) noexcept {
    bufferCheck(count * 3, count * 3);
    setBufferPrimitive(PrimitiveTriangle);
}

MInlineAuto beginTriangleVertices(int vertexCount, int indexHead) noexcept {
    bufferCheck(vertexCount, indexHead);
    setBufferPrimitive(PrimitiveTriangle);
}

MInlineAuto beginLines(int count) noexcept {
    bufferCheck(count * 2, count * 2);
    setBufferPrimitive(PrimitiveLine);
}

MInlineAuto beginPoints(int count) noexcept {
    bufferCheck(count, count);
    setBufferPrimitive(PrimitivePoint);
}

}  // namespace mangl::render
