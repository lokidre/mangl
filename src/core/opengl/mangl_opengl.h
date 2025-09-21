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
#include "mangl_opengl_shader_decl.h"

#if MANGL_OPENGL


#if APEAL_OS_MAC

#if APEAL_OS_IPHONE
    #if MANGL_OPENGL_ES1
        #include <OpenGLES/ES1/gl.h>
        #include <OpenGLES/ES1/glext.h>
        //#define GL_SHADING_LANGUAGE_VERSION GL_VERSION
    #elif MANGL_OPENGL_ES2
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
    #elif MANGL_OPENGL_ES3
        #include <OpenGLES/ES3/gl.h>
        #include <OpenGLES/ES3/glext.h>
    #endif
#else  // Desktop Mac
    #include <OpenGL/gl3.h>
    #include <OpenGL/gl3ext.h>
#endif

#elif APEAL_OS_ANDROID

#if MANGL_OPENGL_ES1
    #define GL_GLEXT_PROTOTYPES 1
    #include <GLES/gl.h>
    #include <GLES/glext.h>
#elif MANGL_OPENGL_ES2
    #define GL_GLEXT_PROTOTYPES 1
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
#elif MANGL_OPENGL_ES3
    #include <GLES3/gl3.h>
    #include <GLES3/gl3ext.h>
#endif

#elif defined(TARGET_OS_WINDOWS)

// GLEW headers must be included before GL headers
#include <GL/mangl_glew.h>
#include <GL/mangl_wglew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#elif APEAL_OS_UNIX

#include <GL/mangl_glew.h>
//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>


#endif

#if !defined(GL_RENDERBUFFER) && defined(GL_RENDERBUFFER_OES)
#define GL_FRAMEBUFFER GL_FRAMEBUFFER_OES
#define GL_RENDERBUFFER GL_RENDERBUFFER_OES
#define GL_RENDERBUFFER_BINDING GL_RENDERBUFFER_BINDING_OES
#define GL_COLOR_ATTACHMENT0 GL_COLOR_ATTACHMENT0_OES
#define GL_FRAMEBUFFER_COMPLETE GL_FRAMEBUFFER_COMPLETE_OES
#define GL_FRAMEBUFFER_BINDING GL_FRAMEBUFFER_BINDING_OES
#define GL_RENDERBUFFER_WIDTH GL_RENDERBUFFER_WIDTH_OES
#define GL_RENDERBUFFER_HEIGHT GL_RENDERBUFFER_HEIGHT_OES
#define GL_DEPTH_COMPONENT16 GL_DEPTH_COMPONENT16_OES
#define GL_DEPTH_ATTACHMENT GL_DEPTH_ATTACHMENT_OES

#define glGenRenderbuffers  glGenRenderbuffersOES
#define glBindRenderbuffer  glBindRenderbufferOES
#define glBindFramebuffer  glBindFramebufferOES
#define glGenFramebuffers   glGenFramebuffersOES
#define glDeleteFramebuffers   glDeleteFramebuffersOES
#define glDeleteRenderbuffers   glDeleteRenderbuffersOES
#define glCheckFramebufferStatus glCheckFramebufferStatusOES
#define glFramebufferRenderbuffer glFramebufferRenderbufferOES
#define glGetRenderbufferParameteriv glGetRenderbufferParameterivOES
#define glRenderbufferStorage glRenderbufferStorageOES
#define glGenerateMipmap glGenerateMipmapOES

#define glGenVertexArrays glGenVertexArraysOES
#define glBindVertexArray glBindVertexArrayOES

#endif

//#if GL_OES_vertex_array_object
//#define glGenVertexArrays glGenVertexArraysOES
//#define glBindVertexArray glBindVertexArrayOES
//#endif

#if MANGL_OPENGL_ES1 || MANGL_OPENGL_ES2
#define glInvalidateFramebuffer glDiscardFramebufferEXT
#endif



namespace mangl::opengl {

#if _DEBUG
void manglAssertOpenGlBody(const char *file, int line);
#define ManglAssertOpenGL() ::mangl::opengl::manglAssertOpenGlBody(__FILE__, __LINE__)
#else  // release build
#define ManglAssertOpenGL()
#endif


struct RenderEngineOpenGL {
    static GLuint glVertexArray;
    static GLuint glVertexBufferObject, glIndexBufferObject;
    static GLsizeiptr glVertexBufferSize, glIndexBufferSize;
    static int glMaxVertices, glMaxIndices;

    // Caching
    static bool cacheVtxPosEnabled_;
    static bool cacheVtxClrEnabled_;
    static bool cacheVtxNrmEnabled_;
    static bool cacheDepthTestEnabled_;
    static bool cacheCullEnabled_;
    static bool cacheVtxTexEnabled_;
    static bool cacheScissorEnabled_;
    static RectGL cacheScissorRect_;


    static ShaderPermutation cachePermutation_;
    static ShaderProgram* cacheShader_;

    static GLfloat cacheScreenScale_;
    static GLfloat cacheLineWidth_;
    static GLfloat cachePointSize_;


    // Screenshots
    static bool screenshotScheduled;
    static SizeI screenshotSize;
    static GLuint screenshotFbo;
    static GLuint screenshotTexture;
    static GLuint screenshotRbo;





    static void renderBegin();
    static void renderCommit();

    static void oglUpdateLineWidth(GLfloat width);
    static void oglUpdatePointSize(GLfloat size);

    static void screenshotPrepare();
    static void screenshotCommit();


};


}  // namespace mangl::render

#endif

