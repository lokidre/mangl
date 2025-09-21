/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#import <Cocoa/Cocoa.h>

#import "mangl_main_view.h"


@interface ManglMainViewController: NSViewController

@property (nonatomic) ManglMainView* runtimeView;

+(instancetype)instance;

// Alert
-(void)showAlert:(NSString *)message withTitle:(NSString *)title;
-(void)showAlert:(NSString *)message;
-(void)showCriticalError:(NSString *)message withTitle:(NSString *)title;
-(void)showCriticalError:(NSString *)message;


@end

