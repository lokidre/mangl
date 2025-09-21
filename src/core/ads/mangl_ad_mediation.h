/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/singleton.hpp>

#include "mangl_ad_mediation_data.h"

#include "../mangl_common.h"
#include "../mangl_ads.h"

#include <apeal/parse/yaml.hpp>
#include <set>
#include <unordered_set>


namespace mangl {

#if MANGL_ADS_MEDIATION

class AdMediation: public apl::Singleton<AdMediation> {
    AdNetworksSetup setup_;

    std::unordered_set<String> enabledNetworks_;
    bool launched_{}, activated_{};
    bool bannerShowRequested_{};
    bool bannerRemoveRequested_{};
    
    //
    // App Open
    //
    bool appOpenShowScheduled_{};
    bool appOpenShowPending_{};
    bool appOpenShowing_{};  // Currently showing app open ad
    

    //
    // Rewarded
    //
    bool rewardedShowRequested_{};
    
    TimerHandler timerHandler_{};

    // Lifetime
    void onTimer(Timestamp timestamp);

public:
    // Initialization
    void setEnabledNetworks(const std::vector<String>& networks) {
        for (auto& name: networks)
            enabledNetworks_.insert(name);
    }
    
    void loadSetup(StrView data, bool showException) {
        loadNetworksSetupData(data, showException);
    }
    
    void onApplicationLaunch() {
        timerHandler_.registerHandler([this](auto timestamp) {
            onTimer(timestamp);
        });
        
        launched_ = true;
    }
    
    void onApplicationActivate() {
        activated_ = true;
    }

    // Banners

    void showBanner() {
        bannerShowRequested_ = true;
        bannerRemoveRequested_ = false;
    }
    
    void removeBanner() {
        bannerRemoveRequested_ = true;
        bannerShowRequested_ = false;
    }

    bool isBannerReady() {
        return setup_.readyBanner != nullptr;
    }

    //
    // Interstitials
    //
    bool isInterstitialReady() noexcept;
    AdStatus getInterstitialStatus() noexcept;
    void showAvailableInterstitial();
    
    //
    // AppOpen
    //
    bool isAppOpenReady() noexcept;
    AdStatus getAppOpenStatus() noexcept;
    bool showAvailableAppOpen();
    bool scheduleAppOpenShow();
    bool dismissAppOpen();

    //
    // Rewarded ads
    //
    bool isRewardedReady() noexcept;
    void showRewarded();


private:

    //
    // Loading setup
    //
    void loadNetworksSetupData(StrView setupData, bool showException);

    void loadNetworksSetupDataApp(const apl::YamlNode& appNode, AdNetworksContainer* networks);
    void loadNetworksSetupDataAdUnit(const apl::YamlNode& unitNode, AdUnitP unit);


    //
    // Periodic Processing
    //
    Timestamp timerCheckTimestamp_{};

    // Maximum simulatanous banner and interstitial requests
    constexpr static int MaxReadyBanners = 2;
    constexpr static int MaxBannerRequests = 6;

    constexpr static int MaxReadyInterstitials = 1;
    constexpr static int MaxInterstitialRequests = 2;






public:
    //
    // Interaction with the Ad SDKs
    //
    //
    // Callbacks
    //
    
    // Initialization
    void onNetworkInitialized(StrView networkName);
    
    // Banners
    void onBannerLoaded(StrView networkName, StrView unitId);
    void onBannerLoadError(StrView networkName, StrView unitId, StrView message);
    void onBannerShowError(StrView networkName, StrView unitId, StrView message){}
    void onBannerShown(StrView networkName, StrView unitId);
    void onBannerClicked(StrView netowrkName, StrView unitId);

    // Interstitials
    void onInterstitialLoaded(StrView networkName, StrView unitId);
    void onInterstitialLoadError(StrView networkName, StrView unitId, StrView message);
    void onInterstitialShowError(StrView networkName, StrView unitId, StrView message);
    void onInterstitialWillPresentFullScreen(StrView networkName, StrView unitId);
    void onInterstitialDidDismissFullScreen(StrView networkName, StrView unitId);

    // App Open
    void onAppOpenLoaded(StrView networkName, StrView unitId);
    void onAppOpenLoadError(StrView networkName, StrView unitId, StrView message);
    void onAppOpenShowError(StrView networkName, StrView unitId, StrView message);
    void onAppOpenWillPresentFullScreen(StrView networkName, StrView unitId);
    void onAppOpenDidDismissFullScreen(StrView networkName, StrView unitId);

    
    // Rewarded
    void onRewardedLoaded(StrView networkName, StrView unitId);
    void onRewardedLoadError(StrView networkName, StrView unitId, StrView message);
    void onRewardedShowError(StrView networkName, StrView unitId, StrView message);
    void onRewardedWillPresentFullScreen(StrView networkName, StrView unitId);
    void onRewardedDidDismissFullScreen(StrView networkName, StrView unitId);
    void onRewardEarned(StrView networkName, StrView unitId, StrView reward, double amount);

    

private:
    //
    // Requests
    //
    void sendRequestNetworkReset(StrView networkName);

    void requestNetworkReset(AdNetworkP network) {
        sendRequestNetworkReset(network->name);
    }

    void requestHighestBidBanner();
    
    void requestBannerLoad(AdUnitP unit);
    void requestBannerShow(AdUnitP unit);
    void requestBannerHide(AdUnitP unit);
    
    void requestBannerLoad(StrView networkName, StrView unitId);
    void requestBannerShow(StrView networkName, StrView unitId);
    void requestBannerHide(StrView networkName, StrView unitId);


    
    void requestInterstitialLoad(AdUnitP unit);
    void requestInterstitialShow(AdUnitP unit);

    void sendRequestInterstitialLoad(StrView networkName, StrView unitId);
    void sendRequestInterstitialShow(StrView networkName, StrView unitId);


    void requestAppOpenLoad(AdUnitP unit);
    void requestAppOpenShow(AdUnitP unit);

    void sendRequestAppOpenLoad(StrView networkName, StrView unitId);
    void sendRequestAppOpenShow(StrView networkName, StrView unitId);
    void sendRequestAppOpenDismiss(StrView networkName, StrView unitId);

    
    
    void requestRewardedLoad(AdUnitP unit);
    void requestRewardedShow(AdUnitP unit);

    void sendRequestRewardedLoad(StrView networkName, StrView unitId);
    void sendRequestRewardedShow(StrView networkName, StrView unitId);

private:
    //
    // Utilities
    //
    AdNetworkP findNetwork(StrView networkName);
    AdUnitP findUnit(StrView networkName, StrView unitId);
    AdUnitP findBannerUnit(StrView networkName, StrView unitId);
    AdUnitP findInterstitialUnit(StrView networkName, StrView unitId);
    AdUnitP findAppOpenUnit(StrView networkName, StrView unitId);
    AdUnitP findRewardedUnit(StrView networkName, StrView unitId);
    Timestamp currentTimestamp_{};
    
    
    // Timer
    void processTimerBanners(Timestamp timestamp);
    void processTimerInterstitials(Timestamp timestamp);
    void processTimerAppOpens(Timestamp timestamp);
};

#else // Dummy

class AdMediation: public apl::Singleton<AdMediation> {
public:
    void onApplicationLaunch(){}
    void onNetworkInitialized(StrView networkName){}

    void onBannerLoaded(StrView networkName, StrView unitId){}
    void onBannerLoadError(StrView networkName, StrView unitId, StrView message){}
    void onBannerShowError(StrView networkName, StrView unitId, StrView message){}
    void onBannerClicked(StrView netowrkName, StrView unitId){}

    void onInterstitialLoaded(StrView networkName, StrView unitId){}
    void onInterstitialLoadError(StrView networkName, StrView unitId, StrView message){}
    void onInterstitialShowError(StrView networkName, StrView unitId, StrView message){}
    void onInterstitialWillPresentFullScreen(StrView networkName, StrView unitId){}
    void onInterstitialDidDismissFullScreen(StrView networkName, StrView unitId){}

    void onAppOpenLoaded(StrView networkName, StrView unitId){}
    void onAppOpenLoadError(StrView networkName, StrView unitId, StrView message){}
    void onAppOpenShowError(StrView networkName, StrView unitId, StrView message){}
    void onAppOpenWillPresentFullScreen(StrView networkName, StrView unitId){}
    void onAppOpenDidDismissFullScreen(StrView networkName, StrView unitId){}

    void onRewardedLoaded(StrView networkName, StrView unitId){}
    void onRewardedLoadError(StrView networkName, StrView unitId, StrView message){}
    void onRewardedShowError(StrView networkName, StrView unitId, StrView message){}
    void onRewardedWillPresentFullScreen(StrView networkName, StrView unitId){}
    void onRewardedDidDismissFullScreen(StrView networkName, StrView unitId){}
    void onRewardEarned(StrView networkName, StrView unitId, StrView reward, double amount){}

};

#endif

}  // namespace mangl
