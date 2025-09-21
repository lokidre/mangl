/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../../core/mangl_setup.h"

#if MANGL_ADS

#include "../../core/mangl_core.h"
#include "../../core/mangl_ads.h"

#import "../mangl_main_vc+ads.h"
#import "../../apple/mangl_imacos_util.h"

namespace mangl {
enum class ManglAdType {
    Banner, Interstitial, AppOpen, Rewarded,
};
} // namespace mangl

@interface ManglAdAdaptor: NSObject {
@public
    MString name;
    bool initialized_;
    
//@protected
}

@property (nonatomic, weak) ManglMainViewController *rootViewController;
@property (nonatomic, weak) UIView *currentParentView;


// Initialization
- (id)init;
- (void)onInitNetwork;
- (bool)isInitialized;

- (void)processAdaptorInitCompletion;

// Interaction with the C++ class
- (void)requestNetworkReset;

- (void)maintentanceFullScreenAds;
- (void)recordFullScreenAdLoad:(NSString*)unitId type:(mangl::ManglAdType)type;
- (void)recordFullScreenAdDisplay:(NSString*)unitId type:(mangl::ManglAdType)type;
- (void)recordFullScreenAdError:(NSString*)unitId type:(mangl::ManglAdType)type;

// Banners
- (void)requestBannerLoad:(MStrView)unitId;
- (void)requestBannerShow:(MStrView)unitId;
- (void)requestBannerRemove;

// Interstitials
- (bool)isInterstitialReady:(MStrView)unitId;
- (void)requestInterstitialLoad:(MStrView)unitId;
- (void)requestInterstitialShow:(MStrView)unitId;

// App Open
- (bool)isAppOpenReady:(MStrView)unitId;
- (void)requestAppOpenLoad:(MStrView)unitId;
- (void)requestAppOpenShow:(MStrView)unitId;
- (void)requestAppOpenDismiss:(MStrView)unitId;


// Rewarded
- (bool)isRewardedReady:(MStrView)unitId;
- (void)requestRewardedLoad:(MStrView)unitId;
- (void)requestRewardedShow:(MStrView)unitId;




@end

#endif // if ads
