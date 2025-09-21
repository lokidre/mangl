/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../../../core/mangl_setup.h"

#if MANGL_ADS_ADMOB || MANGL_ADS_MEDIATION_ADMOB

#import "../mangl_ad_adaptor.h"

constexpr inline MStrView ManglAdNetworkNameAdmob = "admob";

@import GoogleMobileAds;

@interface ManglAdAdaptorAdMob: ManglAdAdaptor<GADBannerViewDelegate, GADFullScreenContentDelegate>
@end


#endif  // If AdMov Ads
