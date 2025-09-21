/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once


namespace mangl::render {

// Cache information about two textures
// to avoid iteration over textures lists
// static TextureId lastTextureId[2]{};
// static RenderTextureInfo* lastTextureInfo[2]{};
// static int lastTextureIndex{};

MInlineAuto findTextureInfo(TextureId tid) noexcept -> RenderTextureInfo*
{
    auto& lastTextureId = pipelineStatics_.lastTextureId;
    auto& lastTextureInfo = pipelineStatics_.lastTextureInfo;
    auto& lastTextureIndex = pipelineStatics_.lastTextureIndex;

    if (tid == lastTextureId[0])
        return lastTextureInfo[0];

    if (tid == lastTextureId[1])
        return lastTextureInfo[1];

    for (auto tip = textures_, end = tip + texturesCount_ ; tip != end ; ++tip) {
        if (auto ti = *tip; ti && ti->valid && tid == ti->textureId) {
            lastTextureId[lastTextureIndex] = tid;
            lastTextureInfo[lastTextureIndex] = ti;
            lastTextureIndex = lastTextureIndex ? 0 : 1;
            return ti;
        }
    }

    MDebugTrap();

    return nullptr;
}


MInlineAuto enableTextures(const RenderTextureInfo *texInfo) noexcept {
    setUniformStateValue(&glState_.texture, texInfo);
}

MInlineAuto disableTextures() noexcept {
    setUniformStateValue(&glState_.texture, nullptr);
}

MInlineAuto processPrimTextureEnable(RenderContextA ctx, TextureA texture, int textureIndex) {
    // Find the texture
    pipelineTextureInfo_ = findTextureInfo(texture.tid);

#if _DEBUG
    if (!pipelineTextureInfo_ || !texture.coords) {
        MDebugTrap();
    }
#endif

    if (!pipelineTextureInfo_)
        return;

    pipelineTextureSize_ = pipelineTextureInfo_->size.convert<SizeGL>();


    auto texCoordPtr = texture.coords + textureIndex * 4;
    auto tex = RectGL::make(std::span{texCoordPtr, 4});

    // The textures are flipped vertically
    pipelineTextureA_ = {tex.x, tex.y + tex.h};
    pipelineTextureD_ = {tex.x + tex.w, tex.y};

    enableTextures(pipelineTextureInfo_);

}

MInlineAuto processPrimTexture(RenderContextA ctx) noexcept
{
    auto& prim = *ctx.prim;

    if (PrimType::Model == prim.type) {
        auto model = prim.model;
        MDebugTrapAssert(model != nullptr);
        if (model->texture) {
            processPrimTextureEnable(ctx, model->texture, model->textureIndex);
        } else {
            disableTextures();
        }
        return;
    }

    if (prim.textureFlag_) {
        processPrimTextureEnable(ctx, prim.texture_, prim.textureIndex_);

//        // Find the texture
//        pipelineTextureInfo_ = findTextureInfo(prim.texture_.tid);
//
//#if _DEBUG
//        if (!pipelineTextureInfo_ || !prim.texture_.coords) {
//            MDebugTrap();
//        }
//#endif
//
//        if (!pipelineTextureInfo_)
//            return;
//
//        pipelineTextureSize_ = pipelineTextureInfo_->size.convert<SizeGL>();
//
//
//        auto texCoordPtr = prim.texture_.coords + prim.textureIndex_ * 4;
//        auto tex = RectGL::make(std::span{texCoordPtr, 4});
//
//        // The textures are flipped vertically
//        pipelineTextureA_ = {tex.x, tex.y + tex.h};
//        pipelineTextureD_ = {tex.x + tex.w, tex.y};
//
//        enableTextures(pipelineTextureInfo_);
    } else {
        disableTextures();
    }

}

}  // namespace mangl::render

