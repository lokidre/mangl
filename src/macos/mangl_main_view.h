/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#import <Cocoa/Cocoa.h>

#include "../core/mangl_types.h"

#import "../apple/mangl_metal_render_view.h"

#include <functional>

bool manglPushInit(const std::function<void()> &f);

@interface ManglMainView: ManglMetalRenderView

- (void)onLaunch;

- (void)onViewport;
- (void)onResumeTextures;
- (void)onResume;
- (void)onPause;
- (void)onTerminate;

- (void)pauseRefresh;
- (void)resumeRefresh;

- (CVReturn)getFrameForTime:(mangl::Timestamp)timestamp;

@end


