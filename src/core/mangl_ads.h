/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_types.h"
#include "mangl_environment.h"
#include "mangl_storage.h"

#include <apeal/singleton.hpp>

namespace mangl {

class Scene;
using SceneP = Scene*;

struct Prim;
using PrimP = Prim*;

struct Widget;
using WidgetP = Widget*;

struct AdsSetupParams {
    StrView schema;
    SceneP scene;
};

struct AdStatus {
    bool ready{};
    int bid{};
    String unitId{};
};

struct Ads {

    // Setup
    static void setup(const AdsSetupParams& params);


    // Status
    static Rect bannerRect;
    static bool bannerHidden;
    static SignalVoid bannerLayoutChangedCallback;
    

#if MANGL_ADS
    static Setting<bool> statePermanentDisabled_; //{"mangl_ads_permanent_disabled", false};
    static bool permanentDisabled_, permanentStatsLoaded_;
    static bool adsEnabled_, adsBannerEnabled_, adsIntersitialEnabled_, appOpenEnabled_;
    constexpr static bool adsSupported() noexcept { return true; }
#else
    constexpr static bool permanentDisabled_{true}, permanentStatsLoaded_{true};
    constexpr static bool adsEnabled_{}, adsBannerEnabled_{}, adsIntersitialEnabled_{}. appOpenEnabled_{};
    constexpr static bool adsSupported() noexcept { return false; }
#endif

//    static bool interstitialAvailable_;
    
    static bool setPermanentEnabled(bool f) {
        auto curr = isPermanentEnabled();
#if MANGL_ADS
        permanentDisabled_ = !f;
        setEnabled(f);
        statePermanentDisabled_ = !f;
        permanentStatsLoaded_ = true;
#endif
        return curr;
    }
    static bool isPermanentEnabled() {
#if MANGL_ADS
        if (!permanentStatsLoaded_) {
            permanentDisabled_ = statePermanentDisabled_.get();
            permanentStatsLoaded_ = true;
        }
#endif
        
        return !permanentDisabled_;
    }
    
    static bool isPermanentDisabled() { return !isPermanentEnabled(); }
    

    static bool setEnabled(bool f) noexcept {
        auto curr = adsEnabled_;
#if MANGL_ADS
        adsEnabled_ = f;
        adsBannerEnabled_ = f;
        adsIntersitialEnabled_ = f;
        appOpenEnabled_ = f;

        if (!f)
            hideBanner();
#endif
        return curr;
    }


    static bool enabled() noexcept {  return isPermanentEnabled() && adsEnabled_; }
    static bool enable() noexcept {  return setEnabled(true); }
    static bool disable() noexcept {  return setEnabled(false);  }

    // Banners
    static void showBanner();
    static void hideBanner();
//    static void unhideBanner();

    static bool bannerEnabled() noexcept { return enabled() && adsBannerEnabled_; }
    static bool isBannerVisible() noexcept { return bannerEnabled() && !bannerHidden; }

    static bool setBannerEnabled(bool f) noexcept { 
        if (!adsSupported() || !isPermanentEnabled())
            return false;

        if (adsBannerEnabled_ == f)
            return adsBannerEnabled_;

#if MANGL_ADS
        if (!adsEnabled_ && f) {
            adsEnabled_ = f;
        }

        adsBannerEnabled_ = f;
        adsBannerEnabled_ ? showBanner() : hideBanner();
#endif

        return adsBannerEnabled_; 
    }

    enum class BannerPlacement {
        Bottom,
        Top,
        Custom,
    };

    constexpr static Size defaultBannerSize() noexcept { return {320, 50}; }


    static void setBannerLayout(Size size, BannerPlacement placement = BannerPlacement::Bottom) noexcept {
        bannerRect = Env::safeLayout().centered(size);
        bannerRect.y = Env::safeLayout().y;
        bannerLayoutChangedCallback();
    }

    static const auto& bannerLayout() noexcept { return bannerRect; }
    static const auto& getBannerRect() noexcept { return bannerRect; }


    // Debugging banners
    struct DebugBannerParams {
        SceneP scene;
        WidgetP widget;
        PrimP prim;
    };
    static DebugBannerParams debugBannerParams;
    static void debugSetBannerPrim(const DebugBannerParams& params);

    //
    // Interstitials
    //
    static bool interstitialEnabled() noexcept {
        return adsEnabled_ && adsIntersitialEnabled_;
    }
    static AdStatus getInterstitialStatus() noexcept;
    static bool isInterstitialReady() noexcept;
    static void showInterstitial();
    static void showInterstitialDelayed(Timestamp delay);

    static std::function<void (StrView)> onInterstitialShown;
    static std::function<void (StrView)> onInterstitialDismissed;


    //
    // App Open
    //
    static bool isAppOpenEnabled() noexcept {
        return adsEnabled_;
    }
//    static AdStatus getAppOpenStatus() noexcept;
    static bool isAppOpenReady() noexcept;
    static void showAppOpen();  // Show app open only if it's ready
    static void scheduleAppOpen();  // Schedule app open show when it's ready
    static void dismissAppOpen();  // Dismisses app open if it's open and cancels scheduled show

    static std::function<void (StrView)> onAppOpenShown;
    static std::function<void (StrView)> onAppOpenDismissed;


    //
    // Rewarded ads
    //
    static bool isRewardedReady() noexcept;
    static void showRewarded();
    static std::function<void (StrView)> onRewardedError;
    static std::function<void (StrView, double)> onRewardEarned;
    
//    struct Reward {
//        String type;
//        int amount;
//    };
//    using RewardContainer = std::vector<Reward>;
//
//    SignalStrView revidLoadedSignal;
//    Signal<StrView, StrView> revidLoadFailedSignal;
//    Signal<StrView, StrView> revidPlayFailedSignal;
//    SignalStrView revidWillAppearSignal, revidDidAppearSignal;
//    SignalStrView revidWillDisappearSignal, revidDidDisappearSignal;
//
//    Signal<StrView, Reward> revidShouldRewardSignal;
//    SignalStrView revidExpiredSignal;
//
//    SignalStrView revidDidTapSignal;
//    SignalStrView revidWillLeaveAppSignal;
//
//
//    static bool rewardedAdAutoplay, rewardedAdCancelled;
//    
//    static void loadRewardedAd(StrView unitId, bool autoplay = false);
//    static bool rewardedAdAvailable(StrView unitId);
//    static RewardContainer availableRewardedAdRewards(StrView unitId);
//    static void showRewardedAd(StrView unitId, const Reward& reward={}, StrView customData={});
//    static void cancelRewardedAd();
};

}  // namespace mangl
