/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_texture_atlas.h"

#include "../mangl_environment.h"
#include "../mangl_common.h"
#include "../mangl_render.h"
#include "../render/mangl_pl_util_global.h"


namespace mangl {


bool TextureAtlas::loaded(StrView filename) const
{
    // Parse and load atlas file
    // Deduce file names (for now)
    // Obsolete
    auto [name, ext] = apl::splitFileExt(filename);

    auto imageFileName = apl::makeFileName(name, M::DefaultTextureImageExt);


    return SceneRenderer::textureLoaded(imageFileName);
}


void TextureAtlas::release()
{
    if (textureId) {
        SceneRenderer::releaseTexture(textureId);
    }
    clear();
}


}  // namespace mangl
