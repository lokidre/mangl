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

#if MANGL_RATE_ANNOYER
@import Foundation;

@interface ManglRateAnnoyer: NSObject {

@public
    int daysToRate, launchesToRate;
    bool debug;          // debug annoyer default is false
    bool trackVersion;   // keep asking for each version
}

@property (nonatomic) NSString *appId, *appName;
@property (nonatomic) NSString *promptTitle, *promptMessage;
@property (nonatomic) NSString *rateButtonTitle, *laterButtonTitle, *declineButtonTitle;

+ (ManglRateAnnoyer *)instance;
+ (void)launch;
+ (void)rate;
+ (void)prompt;
+ (bool)acted;


- (void)clear;   // clears all the keys, for debugging only

@end
#endif

