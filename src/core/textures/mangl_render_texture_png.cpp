/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_texture_atlas.h"

#include "../mangl_common.h"
#include "../mangl_render.h"


#if !defined(APEAL_OS_MAC)

namespace mangl {

void SceneRenderer::loadTextureData(StrView filename, apl::Image* image) {
    *image = apl::loadImage(filename);
}

}  // namespace mangl

#endif  // Windows / Android / Linux

