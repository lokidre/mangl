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

#import "../mangl_main_vc+ads.h"
#import "mangl_ad_adaptor.h"

@interface ManglAdNetwork: NSObject

@property (nonatomic, weak) UIView *currentParentView;
@property (nonatomic, weak) ManglMainViewController *rootViewController;


+ (instancetype)instance;

// Initialization
- (void)onInitNetwork;
- (void)onApplicationDidBecomeActive;
- (void)onApplicationWillResignActive;


// Api
- (void)requestAppOpenLoad:(MStrView)network unitId:(MStrView)unitId;
- (void)requestAppOpenShow:(MStrView)network unitId:(MStrView)unitId;
- (void)requestAppOpenDismiss:(MStrView)network unitId:(MStrView)unitId;


// Utilities
- (CGRect)getBannerRectCG;


// Notifications
- (void)onAdaptorInitialized:(ManglAdAdaptor *)adaptor;

- (void)onBannerLoaded:(ManglAdAdaptor *)adaptor unitId:(NSString *)uniId;
- (void)onBannerLoadError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error;
- (void)onBannerDidShow:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onBannerShowError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error;
- (void)onBannerClicked:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onBannerWillShowFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onBannerDidDismissFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;

- (void)onInterstitialLoaded:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onInterstitialLoadError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error;
- (void)onInterstitialShowError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error;
- (void)onInterstitialWillPresentFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onInterstitialDidDismissFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;

- (void)onAppOpenLoaded:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onAppOpenLoadError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error;
- (void)onAppOpenShowError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error;
- (void)onAppOpenWillPresentFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onAppOpenDidDismissFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;


- (void)onRewardedLoaded:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onRewardedLoadError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error;
- (void)onRewardedShowError:(ManglAdAdaptor *)adaptor unitId:(NSString *)uniId withError:(NSError *)error;
- (void)onRewardedWillPresentFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onRewardedDidDismissFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId;
- (void)onRewardEarned:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId reward:(NSString *)reward amount:(double)amount;




@end

#endif  // if ads

