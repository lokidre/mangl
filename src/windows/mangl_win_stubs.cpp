/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../core/framework/mangl_runloop_timer.h"


//
// URL
//
#include "../core/native/mangl_web.h"
namespace mangl {
bool Web::openUrl(StrView url) { return false; }
bool Web::openApp(StrView appUrl, StrView url) { return false; }
} // namespace mangl


//
// Ads mediation
//
#include "../core/ads/mangl_ad_mediation.h"
#if MANGL_ADS_MEDIATION
namespace mangl {
void AdMediation::sendRequestNetworkReset(StrView networkName){}
void AdMediation::sendRequestInterstitialLoad(StrView networkName, StrView unitId){}
    void AdMediation::requestBannerLoad(StrView networkName, StrView unitId){}
    void AdMediation::requestBannerShow(StrView networkName, StrView unitId){}
    void AdMediation::requestBannerHide(StrView networkName, StrView unitId){}
}  // namespae mangl
#endif


