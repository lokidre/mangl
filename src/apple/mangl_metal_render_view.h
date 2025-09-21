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


#if MANGL_METAL

#if APEAL_OS_IPHONE
@import UIKit;
#else  // Mac OS
@import AppKit;
#endif

namespace mangl {
class SceneRenderer;
class Scene;
}  // namespace mangl

#if APEAL_OS_IPHONE
@interface ManglMetalRenderView: UIView {
#else  // Mac OS
@interface ManglMetalRenderView: NSView {
#endif
    
    mangl::SceneRenderer *render_;
}

- (void)attachRender:(mangl::SceneRenderer *)renderer;
- (void)renderScene;
- (void)updateViewport;

@end


#endif

