/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_ads.h"

#include "../core/framework/mangl_runloop_timer.h"
#include "../core/ads/mangl_ads_client.h"
#include "../core/mangl_scene.h"
#include "../core/mangl_widgets.h"
#include "../core/mangl_dispatch.h"

#if MANGL_ADS_MEDIATION
#include "ads/mangl_ad_mediation.h"
#endif


namespace mangl {

#if MANGL_ADS
Setting<bool> Ads::statePermanentDisabled_{"mangl_a_disabled", false};
bool Ads::permanentDisabled_{false};
bool Ads::permanentStatsLoaded_{false};

bool Ads::adsEnabled_{true};
bool Ads::adsBannerEnabled_{true};
bool Ads::adsIntersitialEnabled_{true};
bool Ads::appOpenEnabled_{true};
#endif

Rect Ads::bannerRect{};
bool Ads::bannerHidden{};
SignalVoid Ads::bannerLayoutChangedCallback{};


std::function<void (StrView)> Ads::onInterstitialShown{};
std::function<void (StrView)> Ads::onInterstitialDismissed{};

std::function<void (StrView)> Ads::onAppOpenShown{};
std::function<void (StrView)> Ads::onAppOpenDismissed{};


std::function<void (StrView)> Ads::onRewardedError{};
std::function<void (StrView, double)> Ads::onRewardEarned{};



//bool Ads::rewardedAdAutoplay{}, rewardedAdCancelled{};

Ads::DebugBannerParams Ads::debugBannerParams{};


void Ads::setup(const AdsSetupParams& params) {
#if MANGL_ADS_EMBED
    EmbedAdsNetworkClient::instance()->setup(params);
#endif
}

void Ads::debugSetBannerPrim(const DebugBannerParams& params) {

#ifndef MANGL_CONSOLE
    debugBannerParams = params;

    if (params.scene && !params.prim && !params.widget) {
        debugBannerParams.prim = params.scene->newBox({M::LightYellow, .6}, {});
    }

    if (params.prim)
        params.prim->move(bannerLayout());
    if (params.widget)
        params.widget->move(bannerLayout());
#endif
}


#if MANGL_ADS

// #if (APEAL_OS_IPHONE || APEAL_OS_ANDROID)
// #else
#if MANGL_ADS_EMBED
// Banner
void Ads::showBanner() { EmbedAdsNetworkClient::instance()->showBanner(); }
void Ads::hideBanner() { EmbedAdsNetworkClient::instance()->hideBanner(); }

// Interstitials
void Ads::showInterstitial() { EmbedAdsNetworkClient::instance()->showInterstitial(); }
void Ads::showInterstitialDelayed(Timestamp timestamp) {
    EmbedAdsNetworkClient::instance()->showInterstitialDelayed(timestamp);
}
#elif (APEAL_OS_IPHONE || APEAL_OS_ANDROID)
#else

void Ads::showBanner() {
    if (debugBannerParams.prim) {
        debugBannerParams.prim->setVisible(true);
    }
    if (debugBannerParams.widget) {
        debugBannerParams.widget->setVisible(true);
    }

    bannerHidden = false;
}

void Ads::hideBanner() {
    if (debugBannerParams.prim) {
        debugBannerParams.prim->setVisible(false);
    }

    if (debugBannerParams.widget) {
        debugBannerParams.widget->setVisible(false);
    }

    bannerHidden = true;
}

// Interstitials
void Ads::showInterstitial() {}
void Ads::showInterstitialDelayed(Timestamp) {}

#endif

#else // Ads are not enabled

// Banner
void Ads::showBanner() {}
void Ads::hideBanner() {}

// Interstitials
bool Ads::isInterstitialReady() noexcept { return false; }
AdStatus Ads::getInterstitialStatus() noexcept { return {}; }
void Ads::showInterstitial() {}
void Ads::showInterstitialDelayed(Timestamp) {}


#endif


#if MANGL_ADS && (APEAL_OS_IPHONE || APEAL_OS_ANDROID)
#else // Ads are not enabled
bool Ads::isRewardedReady() noexcept{ return false; }
void Ads::showRewarded(){}

/*
// Rewarded videos
void Ads::cancelRewardedAd() {}
auto Ads::availableRewardedAdRewards(StrView) -> RewardContainer { return {}; }

// Fake rewarded videos for debugging
void Ads::loadRewardedAd(StrView unitId, bool autoplay) {
    rewardedAdAutoplay = autoplay;

    static bool success = false;
    success = !success;
    RunLoopTimer::fire(2, [id = unitId] {
        auto ads = Ads::instance();
        if (success)
            ads->revidLoadedSignal(id);
        else
            ads->revidLoadFailedSignal(id, "Reward Videos failed");
    });
}
bool Ads::rewardedAdAvailable(StrView) {
    static bool avail = false;
    avail = !avail;
    return avail;
}

void Ads::showRewardedAd(StrView unitId, const Reward&, StrView) {
    RunLoopTimer::fire(2, [id = unitId] { Ads::instance()->revidShouldRewardSignal(id, {}); });
}
*/

#endif

#if MANGL_ADS_MEDIATION


void Ads::showBanner() {
    AdMediation::instance()->showBanner();
}

void Ads::hideBanner() {
    AdMediation::instance()->removeBanner();
}


bool Ads::isInterstitialReady() noexcept {
    return AdMediation::instance()->isInterstitialReady();
}

AdStatus Ads::getInterstitialStatus() noexcept {
    return AdMediation::instance()->getInterstitialStatus();
}


void Ads::showInterstitial() {
    if (!Ads::enabled()) return;
    
    AdMediation::instance()->showAvailableInterstitial();
}

void Ads::showInterstitialDelayed(double delay) {
    if (!Ads::enabled()) return;
    
    if (delay == 0) {
        AdMediation::instance()->showAvailableInterstitial();
        return;
    }
    
    Dispatch::asyncMainDelayed([] {
        AdMediation::instance()->showAvailableInterstitial();
    }, delay);
}


bool Ads::isAppOpenReady() noexcept {
    return AdMediation::instance()->isAppOpenReady();
}

//AdStatus Ads::getAppOpenStatus() noexcept {
//    return AdMediation::instance()->getAppOpenStatus();
//}

void Ads::showAppOpen() {
    if (!Ads::enabled()) return;
    
    AdMediation::instance()->showAvailableAppOpen();
}

void Ads::scheduleAppOpen() {
    if (!Ads::enabled()) return;
    
    AdMediation::instance()->scheduleAppOpenShow();
}

void Ads::dismissAppOpen() {
    if (!Ads::enabled()) return;
    
    AdMediation::instance()->dismissAppOpen();
}



bool Ads::isRewardedReady() noexcept {
    return AdMediation::instance()->isRewardedReady();
}

void Ads::showRewarded() {
    AdMediation::instance()->showRewarded();
}

#endif


} // namespace mangl
