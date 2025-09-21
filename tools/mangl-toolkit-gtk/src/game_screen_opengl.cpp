/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "game_screen_opengl.h"

#include <mangl/mangl.h>
#include "../../../src/core/opengl/mangl_opengl.h"
#include "../../../src/3rdparty/glew/GL/mangl_glew.h"


namespace toolkit {

void GameScreenOpenGL::init() {

    auto result = glewInit();
    if (GLEW_OK != result && GLEW_ERROR_NO_GLX_DISPLAY != result) {
        auto errorString = glewGetErrorString(result);

        mangl::mthrowError("GLEW Error: %s", errorString);
    }

    //apl::verify(GLEW_OK == result || GLEW_ERROR_NO_GLX_DISPLAY == result, "GLEW Error: %s", (const char*)glewGetErrorString(result));

    /*
    //
    // Offscreen framebuffer
    //
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint *)&oldFramebuffer_);
    glGenFramebuffers(1, (GLuint *)&framebuffer_);  ManglAssertOpenGL();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);   ManglAssertOpenGL();

    //
    // Color Renderbuffer
    //
    glGenRenderbuffers(1, (GLuint *)&colorRenderbuffer_);   ManglAssertOpenGL();
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);  ManglAssertOpenGL();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer_);


    GLint width{}, height{};
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

    //
    // Depth Renderbuffer
    //
    glGenRenderbuffers(1, (GLuint *)&depthRenderbuffer_);  ManglAssertOpenGL();
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer_);   ManglAssertOpenGL();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer_);

    buffersAllocated_ = true;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer_);
*/

}

void GameScreenOpenGL::destroy() {
}

}  // namespace toolkit
