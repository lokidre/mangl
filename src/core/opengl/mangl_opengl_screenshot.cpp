/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_opengl.h"
#include "../mangl_render.h"

#if MANGL_OPENGL


namespace mangl::opengl {

void RenderEngineOpenGL::screenshotPrepare() {

    //
    // Create framebuffer
    //
    glGenFramebuffers(1, &screenshotFbo); ManglAssertOpenGL();
    glBindFramebuffer(GL_FRAMEBUFFER, screenshotFbo); ManglAssertOpenGL();

    //
    // Create texture render to
    //
    int currentBoundTexture = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentBoundTexture); ManglAssertOpenGL();

    glGenTextures(1, &screenshotTexture); ManglAssertOpenGL();
    glBindTexture(GL_TEXTURE_2D, screenshotTexture); ManglAssertOpenGL();
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 screenshotSize.w,
                 screenshotSize.h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 nullptr); ManglAssertOpenGL();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); ManglAssertOpenGL();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); ManglAssertOpenGL();

    // Attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenshotTexture, 0); ManglAssertOpenGL();

    glBindTexture(GL_TEXTURE_2D, currentBoundTexture);


    //
    // Depth and Stencil testing
    //
    glGenRenderbuffers(1, &screenshotRbo); ManglAssertOpenGL();
    glBindRenderbuffer(GL_RENDERBUFFER, screenshotRbo); ManglAssertOpenGL();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenshotSize.w, screenshotSize.h); ManglAssertOpenGL();
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screenshotRbo); ManglAssertOpenGL();


    // Check if framebuffer success
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        mthrowError("Framebuffer is not complete");
    }
    //glBindFramebuffer(GL_FRAMEBUFFER, 0); ManglAssertOpenGL();

    glBindFramebuffer(GL_FRAMEBUFFER, screenshotFbo);
}

void RenderEngineOpenGL::screenshotCommit() {
    //glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind FBO to return to the default framebuffer

    auto width = static_cast<GLsizei>(screenshotSize.w);
    auto height = static_cast<GLsizei>(screenshotSize.h);
    std::vector<unsigned char> pixels(width * height * 4); // Assuming RGBA format


    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()); ManglAssertOpenGL();

    /*
    //
    // Read texture data
    //
    //glBindFramebuffer(GL_FRAMEBUFFER, screenshotFbo); ManglAssertOpenGL();
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data()); ManglAssertOpenGL();
    glBindFramebuffer(GL_FRAMEBUFFER, 0); ManglAssertOpenGL();

    //
    // Allocate and free all resources
    //
    */

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    if (screenshotRbo) {
        glDeleteRenderbuffers(1, &screenshotRbo);
        screenshotRbo = 0;
    }

    if (screenshotTexture) {
        glDeleteTextures(1, &screenshotTexture);
        screenshotTexture = 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (screenshotFbo) {
        glDeleteFramebuffers(1, &screenshotFbo);
        screenshotFbo = 0;
    }


    // Save the screenshot
    SceneRenderer::processTakenScreenshot(pixels.data(), apl::ColorModel::RGBA, width, height);
}


}  // namespace mangl::opengl


#endif

