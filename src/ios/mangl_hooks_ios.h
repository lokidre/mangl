/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#import "../core/mangl_types.h"


#ifdef TARGET_OS_IPHONE
@import UIKit;
#else
@import AppKit;
#endif


#ifdef TARGET_OS_IPHONE

// General iOS hook
@protocol ManglIosMainHook <NSObject>

@optional
- (BOOL)didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
- (void)applicationDidBecomeActive;
- (void)applicationWillTerminate;
@end

@interface ManglIosHooks: NSObject

@property (nonatomic) NSMutableArray<id<ManglIosMainHook>>* hooks;

+ (ManglIosHooks*)instance;

- (void)registerHook:(id<ManglIosMainHook>)hook;
@end

#endif

