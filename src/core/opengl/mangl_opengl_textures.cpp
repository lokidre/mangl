/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_opengl.h"

#if MANGL_OPENGL

#include "../mangl_render.h"

#include "../render/mangl_render_vertex_pack.h"
#include "../render/mangl_pl_util_global.h"
#include "../render/mangl_pl_util_header.h"



namespace mangl {

void SceneRenderer::glLoadTextureOpenGL(RenderTextureInfo *ti, TextureId textureId)
{
    using namespace opengl;
    using namespace mangl::render;

    using apl::Image;

    // Load texture from file
    Image image;
    loadTextureData(ti->imageFile, &image);

    ti->size = image.size.convert<SizeI>();

    ti->glTextureId = textureId;
    glGenTextures(1, &ti->glTextureId);  ManglAssertOpenGL();

    //MDebugPrint("Gen textures: %d, %8X", ti->glTextureId, (unsigned long)ti);

    ti->glTextureSlot = ti->index;

    glActiveTexture(GL_TEXTURE0 + ti->glTextureSlot); ManglAssertOpenGL();
    glBindTexture(GL_TEXTURE_2D, ti->glTextureId);  ManglAssertOpenGL();

    switch (image.cm) {
    case Image::Model::RGB:  ti->glInternalFormat = ti->glFormat = GL_RGB;   break;
    case Image::Model::RGBA: ti->glInternalFormat = ti->glFormat = GL_RGBA;  break;
    default:
        MDebugTrap();
        break;
    }

    auto [width, height] = ti->size;

    glTexImage2D(GL_TEXTURE_2D, 0, ti->glInternalFormat, width, height, 0, ti->glFormat,
        GL_UNSIGNED_BYTE, image.buffer.data());    ManglAssertOpenGL();

    // Testing if the dimensions are power of two.
    if ((width & (width - 1)) == 0 && (height & (height - 1)) == 0) {
        glGenerateMipmap(GL_TEXTURE_2D); ManglAssertOpenGL();
    }


    //
    // Setup texture rendering parameters right away
    //
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}

void SceneRenderer::glReleaseTextureOpenGL(RenderTextureInfo *ti)
{
    using namespace opengl;

    // delete the texture from OpenGL
    //MDebugPrint("Del textures: %d, %8X", ti->glTextureId, (unsigned long)ti);


    glDeleteTextures(1, &ti->glTextureId);
    ti->glTextureId = 0;

    //cache_.bound2DTexture = (GLuint)-1;
    //cache_.texParameters[ti->index].clear();
}



}  // namespace mangl::opengl

#endif

