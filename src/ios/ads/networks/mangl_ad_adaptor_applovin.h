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


#if MANGL_ADS_APPLOVIN || MANGL_ADS_MEDIATION_APPLOVIN

#import "../mangl_ad_adaptor.h"

@import AppLovinSDK;

constexpr inline MStrView ManglAdNetworkNameAppLovin = "applovin";

@interface ManglAdAdaptorAppLovin: ManglAdAdaptor<MAAdViewAdDelegate>
@end


#endif  // If ads mediation


