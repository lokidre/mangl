/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_ad_mediation.h"

#include "../mangl_ads.h"

namespace mangl {

#if MANGL_ADS_MEDIATION

void AdMediation::onNetworkInitialized(StrView networkName) {
    auto network = findNetwork(networkName);
    if (!network) return;
    
    network->initialized = true;
    
    MDebugPrint("ADM: Networking initialized: %s", networkName);
}


void AdMediation::onBannerLoaded(StrView networkName, StrView unitId) {
    auto unit = findBannerUnit(networkName, unitId);
    
    if (!unit) return;
    
    unit->setStatusLoaded(currentTimestamp_);
    
//    setup_.haveLoadedBanner = true;

    MDebugPrint("ADM: Banner loaded: (%g) %s - %s", unit->bid, networkName, unitId);
}

void AdMediation::onBannerClicked(StrView networkName, StrView unitId) {
    auto unit = findBannerUnit(networkName, unitId);
    
    if (!unit) return;
    
    unit->clearStatus();
    setup_.bannerShowTimestamp = 0;
    
    if (setup_.shownBanner)
        setup_.bannerShowTimestamp -= setup_.shownBanner->refresh;
    
    MDebugPrint("ADM: Banner clicked: (%g) %s - %s", unit->bid, networkName, unitId);
}


void AdMediation::onBannerLoadError(StrView networkName, StrView unitId, StrView message) {
    auto unit = findBannerUnit(networkName, unitId);
    
    if (!unit) return;
    
    unit->setStatusFailed(message, currentTimestamp_);
    MDebugPrint("ADM: Banner load failed (%g): %s - %s", unit->bid, networkName, unitId);
}

void AdMediation::onBannerShown(StrView networkName, StrView unitId) {
    auto unit = findBannerUnit(networkName, unitId);
    
    if (!unit)
        return;

    MDebugPrint("ADM: Banner shown: (%g) %s - %s", unit->bid, networkName, unitId);
}



//
// MARK: Interstitial
//
void AdMediation::onInterstitialLoaded(StrView networkName, StrView unitId) {
    auto unit = findInterstitialUnit(networkName, unitId);
    if (!unit) return;
    
    MDebugPrint("ADM: Interstitial loaded: (%g) %s - %s", unit->bid, networkName, unitId);

    unit->setStatusLoaded(currentTimestamp_);
}

void AdMediation::onInterstitialLoadError(StrView networkName, StrView unitId, StrView message) {
    auto unit = findInterstitialUnit(networkName, unitId);
    if (!unit) return;
    MDebugPrint("ADM: Interstitial load failed: (%g) %s - %s", unit->bid, networkName, unitId);

    unit->setStatusFailed(message, currentTimestamp_);
}

void AdMediation::onInterstitialShowError(StrView networkName, StrView unitId, StrView message) {
    onInterstitialLoadError(networkName, unitId, message);
}


void AdMediation::onInterstitialWillPresentFullScreen(StrView networkName, StrView unitId) {
    auto unit = findInterstitialUnit(networkName, unitId);
    
    if (!unit) return;

    MDebugPrint("ADM: Interstitial shown: (%g) %s - %s", unit->bid, networkName, unitId);

    unit->setStatusShown(currentTimestamp_);
    
}


void AdMediation::onInterstitialDidDismissFullScreen(StrView networkName, StrView unitId) {
    auto unit = findInterstitialUnit(networkName, unitId);
    if (!unit) return;
    
    MDebugPrint("ADM: Interstitial dismissed: (%g) %s - %s", unit->bid, networkName, unitId);
 
    unit->clearStatus();
}


//
// MARK: AppOpen
//
void AdMediation::onAppOpenLoaded(StrView networkName, StrView unitId) {
    auto unit = findAppOpenUnit(networkName, unitId);
    if (!unit) return;
    
    MDebugPrint("ADM: AppOpen loaded: (%g) %s - %s", unit->bid, networkName, unitId);

    unit->setStatusLoaded(currentTimestamp_);
    
    if (appOpenShowScheduled_) {
        requestAppOpenShow(unit);
        appOpenShowScheduled_ = false;
        appOpenShowPending_ = true;
    }
}

void AdMediation::onAppOpenLoadError(StrView networkName, StrView unitId, StrView message) {
    auto unit = findAppOpenUnit(networkName, unitId);
    if (!unit) return;
    MDebugPrint("ADM: AppOpen load failed: (%g) %s - %s", unit->bid, networkName, unitId);

    unit->setStatusFailed(message, currentTimestamp_);
}

void AdMediation::onAppOpenShowError(StrView networkName, StrView unitId, StrView message) {
    appOpenShowPending_ = false;
    appOpenShowScheduled_ = false;
    appOpenShowing_ = false;
    
    onAppOpenLoadError(networkName, unitId, message);
}


void AdMediation::onAppOpenWillPresentFullScreen(StrView networkName, StrView unitId) {
    appOpenShowing_ = true;
    appOpenShowPending_ = false;
    appOpenShowScheduled_ = false;
    
    auto unit = findAppOpenUnit(networkName, unitId);
    
    if (!unit) return;

    MDebugPrint("ADM: AppOpen shown: (%g) %s - %s", unit->bid, networkName, unitId);

    unit->setStatusShown(currentTimestamp_);
}


void AdMediation::onAppOpenDidDismissFullScreen(StrView networkName, StrView unitId) {
    appOpenShowing_ = false;
    appOpenShowPending_ = false;
    appOpenShowScheduled_ = false;

    
    auto unit = findAppOpenUnit(networkName, unitId);
    if (!unit) return;
    
    MDebugPrint("ADM: AppOpen dismissed: (%g) %s - %s", unit->bid, networkName, unitId);
 
    unit->clearStatus();
}



//
// MARK: Rewarded
//
void AdMediation::onRewardedLoaded(StrView networkName, StrView unitId) {
    MDebugPrint("ADM: Rewarded loaded: %s - %s", networkName, unitId);
    
    auto unit = findRewardedUnit(networkName, unitId);
    if (!unit) return;

    unit->setStatusLoaded(currentTimestamp_);
    
    if (rewardedShowRequested_) {
        rewardedShowRequested_ = false;
        sendRequestRewardedShow(networkName, unitId);
        unit->setStatusShown(currentTimestamp_);
    }
}

void AdMediation::onRewardedLoadError(StrView networkName, StrView unitId, StrView message) {
    auto unit = findRewardedUnit(networkName, unitId);
    if (!unit) return;
    
    MDebugPrint("ADM: Rewarded load error: (%g) %s - %s", unit->bid, networkName, unitId);

    unit->setStatusFailed(message, currentTimestamp_);

}

void AdMediation::onRewardedShowError(StrView networkName, StrView unitId, StrView message) {
    onRewardedLoadError(networkName, unitId, message);
}




void AdMediation::onRewardEarned(StrView networkName, StrView unitId, StrView reward, double amount) {
    auto unit = findRewardedUnit(networkName, unitId);
    if (!unit) return;
    
    MDebugPrint("ADM: Rewarded reward: %s - %s, %s: %g", networkName, unitId, reward, amount);
    
    unit->setStatusShown(currentTimestamp_);

    rewardedShowRequested_ = false;
    
//    if (Ads::onRewardEarned) Ads::onRewardEarned(reward, amount);
}



#endif

}  // namespace mangl

