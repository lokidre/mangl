/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_render.h"
#include "../mangl_common.h"

#include "../mangl_scene.h"
#include "../mangl_texture_atlas.h"

#include "../render/mangl_pl_util_global.h"

#include <set>

namespace mangl {

//
// Callback from the render thread
//
void SceneRenderer::onRenderTexturesMaintenance() noexcept
{
    // Walk through the textures
    using namespace render;

    auto tip = textures_;
    for (auto index = 0; index < texturesCount_; ++index, ++tip) {
        if (auto ti = *tip; ti && ti->valid) {
            // Check release first, prim we by accident don't do anything else
            if (ti->pendingRelease) {
                ti->pendingRelease = false;

                // Delete texture
                glReleaseTexture(ti);

                // we are done with this texture, nothing else todo
                ti->clear();
            }

            if (ti->pendingPause) {
                ti->pendingPause = false;

                auto textureId = ti->textureId;
                glReleaseTexture(ti);
                ti->textureId = textureId;
            }
        }
    }



    tip = textures_;
    for (auto index = 0; index < texturesCount_; ++index, ++tip) {
        if (auto ti = *tip; ti && ti->valid) {
            if (ti->pendingLoad) {
                ti->pendingLoad = false;
                glLoadTexture(ti);
            }

            if (ti->pendingResume) {
                ti->pendingResume = false;
                glLoadTexture(ti, ti->textureId);
            }
        }
    }
}

//
// Texture Info list maintenance
//

RenderTextureInfo *SceneRenderer::texInfoCreate()
{
    using namespace render;

    auto tip = textures_;
    for (auto index = 0; index < texturesCount_; ++index, ++tip) {
        if (auto ti = *tip; ti) {
            if (!ti->valid) {  // found empty
                ti->clear();
                ti->textureId = currentTextureId_++;
                return ti;
            }
        }
    }

    // if we are here it means we didn't find available texture
    apl::verify(texturesCount_ < MaxTextures - 1, "Maximum number of textures reached");

    auto& ti = textures_[texturesCount_];

    if (!ti) {
        ti = new RenderTextureInfo;
        ti->valid = true;
        ti->index = texturesCount_;
        ti->textureId = currentTextureId_++;
    }

    // Now increase number of textures
    texturesCount_++;

    return ti;
}

RenderTextureInfo *SceneRenderer::texInfoById(TextureId tid) noexcept
{
    using namespace render;

    for (auto tip = textures_, end = tip + texturesCount_; tip != end; ++tip) {
        if (auto ti = *tip; ti && ti->valid && ti->textureId == tid)
            return ti;
    }

    return nullptr;
}


bool SceneRenderer::texInfoFindIf(const std::function<bool(RenderTextureInfo *)>& f) {
    using namespace render;

    for (auto tip = textures_, end = tip + texturesCount_; tip != end; ++tip) {
        if (auto ti = *tip; ti && ti->valid)
            if (f(ti))
                return true;
    }

    return false;
}

void SceneRenderer::texInfoForEach(const std::function<void(RenderTextureInfo *)>& f) {
    using namespace render;

    for (auto tip = textures_, end = tip + texturesCount_; tip != end; ++tip) {
        if (auto ti = *tip; ti && ti->valid)
            f(ti);
    }
}

bool SceneRenderer::textureLoaded(StrView filename) {
    return texInfoFindIf([filename](auto ti) { 
        return !ti->pendingRelease && ti->imageFile == filename; 
    });
}

TextureId SceneRenderer::loadTexture(StrView filename)
{
#if _DEBUG  // Check if the same texture file was already loaded. Waste of resources
    texInfoForEach([&filename](auto ti) { 
        apl::verify(ti->pendingRelease || ti->imageFile != filename, "Texture file already in use: %s", filename);
    });
#endif

    auto ti = texInfoCreate();

    ti->valid = true;
    ti->pendingLoad = true;
    ti->imageFile = filename;

    return ti->textureId;
}

void SceneRenderer::releaseTexture(TextureId tid)
{
    //ADebugPrint("Releasing texture: %d", tid);

    auto texInfo = texInfoById(tid);

    ADebugTrapAssert(texInfo);//, "releaseTexture: Texture doesn't exist");

    if (!texInfo)
        return;

    texInfo->pendingRelease = true;
}

void SceneRenderer::destroyTexturesAll(){
    render::texturesCount_ = 0;
}


void SceneRenderer::releaseTexturesAll() {
    texInfoForEach([](auto ti) { ti->pendingRelease = true; });
}


static std::set<RenderTextureInfo*> pausedTextures_;

void SceneRenderer::pauseTextures()
{
    MDebugFunc();

    using namespace render;

    pausedTextures_.clear();

    for (auto tip = textures_, end = tip + texturesCount_; tip != end; ++tip) {
        if (auto ti = *tip; ti && ti->valid) {
            if (!ti->pendingPause) {
                pausedTextures_.insert(ti);
                ti->pendingPause = true;
            }
        }
    }

}

void SceneRenderer::resumeTextures()
{
    MDebugFunc();

    if (pausedTextures_.empty())
        return;

    for (auto it: pausedTextures_) {
        it->pendingResume = true;
    }

    //pausedTextures_.clear();
}


}  // namespace mangl

