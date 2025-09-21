/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_ad_mediation.h"

namespace mangl {

#if MANGL_ADS_MEDIATION

void AdMediation::requestHighestBidBanner() {
    
    auto& timestamp = currentTimestamp_;
    
    
    for (auto& [networkId, network]: setup_.networks) {
        network->bannersReadyCount = 0;
        network->bannersPendingCount = 0;
    }
    
    for (auto& [bid, unit]: setup_.bannersByBid) {
        auto network = unit->network;
        
        if (unit->loaded || unit->shown) {
            network->bannersReadyCount++;
            
//            if (!setup_.readyBanner)
//                setup_.readyBanner = unit;
//            ++readyUnitsCount;
        }
        
        if (unit->loadPending) {
//            ++pendingUnitsCount;
            network->bannersPendingCount++;
        }
    }

    //    if (readyUnitsCount < MaxReadyBanners && pendingUnitsCount < MaxBannerRequests) {
        // Request the highest bidding banner for the network
            for (auto& [bid, unit]: setup_.bannersByBid) {
                auto network = unit->network;
                
                if (!network->initialized)
                    continue;
                    
                if (unit->loaded)
                    continue;
                if (unit->loadPending)
                    continue;
                
                if (setup_.shownBanner && setup_.shownBanner->shown) {
                    if (setup_.shownBanner->network == network && setup_.shownBanner->unitId == unit->unitId)
                        continue;
                }
                
                // FailRetryPeriod = 120
                if (unit->loadFailed && timestamp < unit->failTimestamp + 30)
                    continue;
                if (timestamp < network->bannerRequestTimestamp + network->bannerRequestPeriod)
                    continue;
                
                if (network->bannersReadyCount >= 1 || network->bannersPendingCount >= 1)
                    continue;
                
                MDebugPrint("ADM: Banner request load: (%g) %s - %s", unit->bid, network->name, unit->unitId);
                requestBannerLoad(unit);
                network->bannersPendingCount++;
                break;
                
                
    //            ++pendingUnitsCount;
                
    //            if (pendingUnitsCount >= MaxBannerRequests)
    //                break;
            }
      //  }

    
}


void AdMediation::requestBannerLoad(AdUnitP unit) {
    unit->loaded = false;
    unit->loadFailed = false;
    unit->loadPending = true;
    unit->network->bannerRequestTimestamp = currentTimestamp_;
    requestBannerLoad(unit->network->name, unit->unitId);
}

void AdMediation::requestBannerShow(AdUnitP unit) {
    setup_.bannerShowTimestamp = currentTimestamp_;
    setup_.shownBanner = unit;
    unit->shown = true;
    unit->showTimestamp = true;
    requestBannerShow(unit->network->name, unit->unitId);
}

void AdMediation::requestBannerHide(AdUnitP unit) {
    unit->loaded = false;
    unit->loadPending = false;
    unit->loadFailed = false;
    unit->shown = false;
    requestBannerHide(unit->network->name, unit->unitId);
}



#endif
} // namespace mangl
