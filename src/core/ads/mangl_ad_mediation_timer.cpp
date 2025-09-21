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

void AdMediation::onTimer(Timestamp timestamp) {
//    MDebugFunc();
    currentTimestamp_ = timestamp;

    if (bannerRemoveRequested_) {
        bannerRemoveRequested_ = false;
        
        if (setup_.shownBanner) {
            requestBannerHide(setup_.shownBanner);
            setup_.shownBanner = nullptr;
        }
        
        // Just in case request to remove all banners
        for (auto& [bid, unit]: setup_.bannersByBid) {
            requestBannerHide(unit);
        }
    }

    

    
    if (!Ads::enabled())
        return;
        
    
    if (timestamp < timerCheckTimestamp_)
        return;

    timerCheckTimestamp_ = timestamp + 1;  // Some delay

    // See if have to schedule banner load
    if (Ads::adsBannerEnabled_) {
        processTimerBanners(timestamp);
    }


    if (Ads::adsIntersitialEnabled_) {
        processTimerInterstitials(timestamp);
    }
    
    if (Ads::appOpenEnabled_) {
        processTimerAppOpens(timestamp);
    }


}

void AdMediation::processTimerBanners(Timestamp timestamp) {
    
    //
    // Show ready banner
    //
    if (setup_.readyBanner == nullptr && bannerShowRequested_) {
        // Check next ready banner
        for (auto& [bid, unit]: setup_.bannersByBid) {
            if (unit->loaded) {
                setup_.readyBanner = unit;
                requestBannerShow(setup_.readyBanner);
                break;
            }
        }
    }
    
    
    if (!bannerShowRequested_)
        return;
   
    //
    // Do some maintenance
    //
    if (setup_.shownBanner) {
        if (timestamp >= setup_.bannerShowTimestamp + setup_.shownBanner->refresh) {
            if (setup_.shownBanner->shown) {
                if (setup_.shownBanner->loaded) {
                    setup_.shownBanner->loaded = false;
                    setup_.shownBanner->network->bannersReadyCount--;
                }
                setup_.shownBanner->shown = false;
                
                if (bannerShowRequested_) {
                    MDebugPrint("ADM: Banner requesting refresh: %g sec", setup_.shownBanner->refresh);
                    requestHighestBidBanner();
                }
            }
            
            
            // Look for another ready banner
            for (auto& [bid, unit]: setup_.bannersByBid) {
                auto network = unit->network;
                if (!unit->loaded)  continue;
//                if (unit == setup_.shownBanner)
//                    continue;
                
                // Found banner
                if (bannerShowRequested_) {
                    MDebugPrint("ADM: Banner switching refresh: %g sec", setup_.shownBanner->refresh);
                    
                    requestBannerHide(setup_.shownBanner);
                    setup_.shownBanner = nullptr;
                    
                    MDebugPrint("ADM: Banner request show: (%g) %s - %s", bid, network->name, unit->unitId);
                    requestBannerShow(unit);
                    setup_.shownBanner = unit;
                    setup_.bannerShowTimestamp = timestamp;
                    break;
                }
            }
            //bannerShowTimestamp_)
        }
    }
    

    requestHighestBidBanner();

}

void AdMediation::processTimerInterstitials(Timestamp timestamp) {
    
    int readyUnitsCount{};
    int pendingUnitsCount{};
    
    for (auto& [id, network]: setup_.networks) {
        network->interReadyCount = 0;
        network->interPendingCount = 0;
    }
    
    for (auto& [bid, unit]: setup_.interstitialsByBid) {
        if (!unit->network->initialized)
            continue;
        if (unit->loaded) {
            auto loadedDelta = timestamp - unit->loadTimestamp;
            // Discard interstitials loaded more than an hour ago
            if (loadedDelta >= 60 * 59) {
                unit->clearStatus();
                continue;
            }
            ++readyUnitsCount;
            unit->network->interReadyCount++;
        }
        
        if (unit->loadPending) {
            ++pendingUnitsCount;
            unit->network->interPendingCount++;
        }
    }

    for (auto& [bid, unit]: setup_.interstitialsByBid) {
        if (!unit->network->initialized)  continue;
        if (unit->loaded || unit->loadPending)  continue;
        
        // FailRetryPeriod = 120
        if (unit->loadFailed && timestamp < unit->failTimestamp + 60)
            continue;
        
        auto network = unit->network;
        if (network->interPendingCount > 0)
            continue;
//        if (network->interReadyCount > 0 || network->interPendingCount > 0)
//            continue;
        
        MDebugPrint("ADM: Inter request load: (%g) %s - %s", unit->bid, unit->network->name, unit->unitId);
        requestInterstitialLoad(unit);
        network->interPendingCount++;
    }
   

}


void AdMediation::processTimerAppOpens(Timestamp timestamp) {
    
    int readyUnitsCount{};
    int pendingUnitsCount{};
    
    for (auto& [id, network]: setup_.networks) {
        network->appOpenReadyCount = 0;
        network->appOpenPendingCount = 0;
    }
    
    for (auto& [bid, unit]: setup_.appopensByBid) {
        if (!unit->network->initialized)
            continue;
        if (unit->loaded) {
            auto loadedDelta = timestamp - unit->loadTimestamp;
            // Discard interstitials loaded more than an hour ago
            if (loadedDelta >= 60 * 59) {
                unit->clearStatus();
                continue;
            }
            ++readyUnitsCount;
            unit->network->appOpenReadyCount++;
        }
        
        if (unit->loadPending) {
            ++pendingUnitsCount;
            unit->network->appOpenPendingCount++;
        }
    }

    for (auto& [bid, unit]: setup_.appopensByBid) {
        if (!unit->network->initialized)  continue;
        if (unit->loaded || unit->loadPending)  continue;
        
        // FailRetryPeriod = 120
        if (unit->loadFailed && timestamp < unit->failTimestamp + 60)
            continue;
        
        auto network = unit->network;
        if (network->interPendingCount > 0)
            continue;
//        if (network->interReadyCount > 0 || network->interPendingCount > 0)
//            continue;
        
        MDebugPrint("ADM: App open request load: (%g) %s - %s", unit->bid, unit->network->name, unit->unitId);
        requestAppOpenLoad(unit);
        network->appOpenPendingCount++;
    }
   

}


#endif

}  // namespace mangl


