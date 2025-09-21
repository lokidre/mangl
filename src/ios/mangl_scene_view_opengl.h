/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_setup.h"

#import <UIKit/UIKit.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>


#if MANGL_OPENGL

namespace mangl {
class SceneRenderer;
struct Scene;
}

@interface ManglSceneViewOpenGL: UIView {

@private
    EAGLContext *oldContext_, *context_;

    bool buffersAllocated_;

    GLuint framebuffer_;
    GLuint oldFramebuffer_;

    GLuint depthRenderbuffer_, colorRenderbuffer_;

    mangl::SceneRenderer *render_;

@public   // options
    bool transparent;
}

+ (ManglSceneViewOpenGL *)instance;

- (void)ctor;
- (void)dtor;

// rendering initialization
- (void)attachRender:(mangl::SceneRenderer *)renderer;


// rendering
- (void)renderScene;

@end

#endif

