/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_pl_util_global.h"
#include "mangl_pl_util_header.h"


namespace mangl::render {

MInlineAuto bufferNeedsFlush(int primType) noexcept 
{
    if (!pipelineStatics_.primitiveType || pipelineStatics_.indexHead == pipelineStatics_.indexStart)
        return false;

    if (primType != pipelineStatics_.primitiveType)
        return true;

    //if (!glLastState_.valid) {
    //    glLastState_ = glState_;
    //    return false;
    //}

    //auto res = std::memcmp(&glState_, &glLastState_, sizeof(glState_));

    //if (res)
    //    return true;

    return false;
}

MInlineAuto bufferFlushPrimitive() noexcept
{
    if (!pipelineStatics_.primitiveType || pipelineStatics_.indexHead == pipelineStatics_.indexStart)
        return;

    if (pipelineStatics_.commandHead >= drawCommands_.ssize())
        drawCommands_.resize(pipelineStatics_.commandHead + 10);

    auto cmd = drawCommands_.data() + pipelineStatics_.commandHead;

    cmd->batchIndex = pipelineStatics_.currBatchIndex;

    cmd->primitiveType = pipelineStatics_.primitiveType;
    cmd->state = glState_;
    
    cmd->vtxStart = pipelineStatics_.vertexStart - pipelineStatics_.batchVtxStart;
    cmd->vtxCount = pipelineStatics_.vertexHead - pipelineStatics_.vertexStart;

    cmd->idxStart = pipelineStatics_.indexStart - pipelineStatics_.batchIdxStart;
    cmd->idxCount = pipelineStatics_.indexHead - pipelineStatics_.indexStart;

    pipelineStatics_.primitiveType = 0;
    
    pipelineStatics_.vertexStart = pipelineStatics_.vertexHead;
    pipelineStatics_.indexStart = pipelineStatics_.indexHead;

    ++pipelineStatics_.commandHead;
}


MInlineAuto bufferFlush() noexcept
{
    bufferFlushPrimitive();

    if (pipelineStatics_.currBatch) {
        pipelineStatics_.currBatch->vtxCount = pipelineStatics_.vertexHead - pipelineStatics_.currBatch->vtxStart;
        pipelineStatics_.currBatch->idxCount = pipelineStatics_.indexHead - pipelineStatics_.currBatch->idxStart;
        pipelineStatics_.currBatch->cmdCount = pipelineStatics_.commandHead - pipelineStatics_.currBatch->cmdStart;
    }
}

// Called once the rendering pipeline is done
MInlineAuto bufferPipelineFinish() noexcept
{
    bufferFlush(); // flush the remaining primitive

    auto vertexPtr = vertexBuffer_.data();
    auto indexPtr = indexBuffer_.data();
    auto cmdPtr = drawCommands_.data();

    auto&& batchBegin = drawBatches_.data();
    auto&& batchEnd = batchBegin + pipelineStatics_.batchHead;

    for (auto batch = batchBegin; batch != batchEnd; ++batch) {
        batch->vtxData = vertexPtr;
        vertexPtr += batch->vtxCount;

        batch->idxData = indexPtr;
        indexPtr += batch->idxCount;

        batch->cmdData = cmdPtr;
        cmdPtr += batch->cmdCount;
    }
}



MInlineAuto bufferCheck(int vertexCount, int idxCount) noexcept
{
    bool newBatch = pipelineStatics_.currBatch == nullptr;

    auto reqVtxBuffer = pipelineStatics_.vertexHead + vertexCount;
    auto reqIdxBuffer = pipelineStatics_.indexHead + idxCount;

    if (reqVtxBuffer > vertexBuffer_.ssize()) {
        vertexBuffer_.resize(reqVtxBuffer);
        vertexData_ = vertexBuffer_.data();
    }

    if (reqIdxBuffer > indexBuffer_.ssize()) {
        indexBuffer_.resize(reqIdxBuffer);
        indexData_ = indexBuffer_.data();
    }

    if (reqVtxBuffer - pipelineStatics_.batchVtxStart > maxBatchVtxCount_ ||
        reqIdxBuffer - pipelineStatics_.batchIdxStart > maxBatchIdxCount_) {
        newBatch = true;
    }


    if (newBatch) {
        bufferFlush();

        if (pipelineStatics_.batchHead >= drawBatches_.ssize())
            drawBatches_.resize(pipelineStatics_.batchHead + 1);

        pipelineStatics_.currBatchIndex = pipelineStatics_.batchHead;
        ++pipelineStatics_.batchHead;

        pipelineStatics_.currBatch = drawBatches_.data() + pipelineStatics_.currBatchIndex;

        pipelineStatics_.currBatch->vtxStart = pipelineStatics_.batchVtxStart = pipelineStatics_.vertexHead;
        pipelineStatics_.currBatch->vtxData = vertexBuffer_.data() + pipelineStatics_.batchVtxStart;

        pipelineStatics_.currBatch->idxStart = pipelineStatics_.batchIdxStart = pipelineStatics_.indexHead;
        pipelineStatics_.currBatch->idxData = indexBuffer_.data() + pipelineStatics_.batchIdxStart;

        pipelineStatics_.currBatch->cmdStart = pipelineStatics_.commandHead;
        pipelineStatics_.currBatch->cmdData = drawCommands_.data() + pipelineStatics_.currBatch->cmdStart;
    }


}



MInlineAuto setBufferPrimitive(int primitiveType) noexcept
{
    if (pipelineStatics_.primitiveType && bufferNeedsFlush(primitiveType)) {
        //bufferFlushPrimitive();
        bufferFlush();
    }

    // we are ready for the next primitive
    pipelineStatics_.primitiveType = primitiveType;
}


template <class MemberT, class ValT>
MInlineAuto setUniformStateValue(MemberT* member, ValT val) {

    if (!pipelineStatics_.primitiveType || pipelineStatics_.indexHead == pipelineStatics_.indexStart) {
        *member = val;
        return;
    }

    if (*member == val)
        return;

    bufferFlush();

    *member = val;
}

}  // namespace mangl::render

