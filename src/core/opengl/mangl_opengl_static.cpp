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


namespace mangl::opengl {

GLuint RenderEngineOpenGL::glVertexArray{};
GLuint RenderEngineOpenGL::glVertexBufferObject{};
GLuint RenderEngineOpenGL::glIndexBufferObject{};
GLsizeiptr RenderEngineOpenGL::glVertexBufferSize{};
GLsizeiptr RenderEngineOpenGL::glIndexBufferSize{};
int RenderEngineOpenGL::glMaxVertices{};
int RenderEngineOpenGL::glMaxIndices{};


// Caching
bool RenderEngineOpenGL::cacheVtxPosEnabled_{};
bool RenderEngineOpenGL::cacheVtxClrEnabled_{};
bool RenderEngineOpenGL::cacheVtxNrmEnabled_{};
bool RenderEngineOpenGL::cacheDepthTestEnabled_{};
bool RenderEngineOpenGL::cacheCullEnabled_{};
bool RenderEngineOpenGL::cacheVtxTexEnabled_{};
bool RenderEngineOpenGL::cacheScissorEnabled_{};
RectGL RenderEngineOpenGL::cacheScissorRect_{};


ShaderPermutation RenderEngineOpenGL::cachePermutation_{};
ShaderProgram* RenderEngineOpenGL::cacheShader_{};

GLfloat RenderEngineOpenGL::cacheScreenScale_{};
GLfloat RenderEngineOpenGL::cacheLineWidth_{};
GLfloat RenderEngineOpenGL::cachePointSize_{};

bool RenderEngineOpenGL::screenshotScheduled{};

SizeI RenderEngineOpenGL::screenshotSize{};
GLuint RenderEngineOpenGL::screenshotFbo{};
GLuint RenderEngineOpenGL::screenshotTexture{};
GLuint RenderEngineOpenGL::screenshotRbo{};


}  // namespace mangl::opengl

#endif

