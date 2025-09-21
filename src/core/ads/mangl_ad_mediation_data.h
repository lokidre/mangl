/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"

#include <map>

namespace mangl {

struct AdNetwork;
using AdNetworkP = AdNetwork*;

struct AdUnit;
using AdUnitP = AdUnit*;

enum class AdUnitType {
    Unknown,
    Banner,
    Interstitial,
    AppOpen,
    Rewarded,
};

struct AdUnit {
    AdUnitType type{AdUnitType::Unknown};
    AdNetworkP network{};
    String unitId;
    Real bid{};
    Timestamp refresh{};

    // Loading
    bool loaded{};
    bool loadPending{};
    bool loadFailed{};

    // Showing
    bool shown{};
    bool showPending{};
    bool showFailed{};
    
    String errorMessage;
    Timestamp loadTimestamp{};
    Timestamp failTimestamp{};
    Timestamp showTimestamp{};
    
    void clearStatus() {
        loadPending = false;
        loadFailed = false;
        loaded = false;
        
        shown = false;
        showPending = false;
        showFailed = false;
    }
    
    void setStatusLoaded(Timestamp timestamp) {
        loadPending = false;
        loadFailed = false;
        loaded = true;
        
        shown = false;
        showPending = false;
        showFailed = false;
        loadTimestamp = timestamp;
    }
    
    void setStatusFailed(StrView msg, Timestamp timestamp) {
        loadFailed = true;
        loadPending = false;
        loaded = false;
        
        shown = false;
        showPending = false;
        showFailed = false;
        
        errorMessage = msg;
        failTimestamp = timestamp;
    }
    
    void setStatusShown(Timestamp timestamp) {
        shown = true;
        
        showPending = false;
        showFailed = false;
        showTimestamp = timestamp;
    }

};


struct AdNetwork {
    using AdUnitsContainer = std::map<String, std::shared_ptr<AdUnit>>;
    
    String name;
    
    AdUnitsContainer banners;
    AdUnitsContainer interstitials;
    AdUnitsContainer appopens;
    AdUnitsContainer rewarded;

    // Setup
    Timestamp bannerRequestPeriod{3};
    
    // Runtime status
    bool initialized{};

    // Banners
    int bannersPendingCount{};
    int bannersReadyCount{};
    Timestamp bannerRequestTimestamp{};

    // Interstitials
    Timestamp interstitialRequestTimestamp{};
    Timestamp interstitialShowTimestamp{};

    int interPendingCount{};
    int interReadyCount{};
    
    // AppOpen
    Timestamp appOpenRequestTimestamp{};
    Timestamp appOpenShowTimestamp{};
    int appOpenPendingCount{};
    int appOpenReadyCount{};

    
};

using AdNetworksContainer = std::map<String, std::shared_ptr<AdNetwork>>;


struct AdNetworksSetup {
    AdNetworksContainer networks;

    using UnitsByBidContainer = std::multimap<Real, AdUnitP, std::greater_equal<Real>>;

    UnitsByBidContainer bannersByBid, interstitialsByBid, appopensByBid, rewardedByBid;


    //
    // Current state
    //
    AdUnitP readyBanner{};
    AdUnitP shownBanner{};
    Timestamp bannerShowTimestamp{};
};


}  // namespace mangl
