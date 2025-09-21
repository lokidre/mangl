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

bool AdMediation::isInterstitialReady() noexcept {
    for (auto& [bid, unit]: setup_.interstitialsByBid) {
        if (unit->loaded && !unit->showPending && !unit->shown)
            return true;
    }
    
    return false;
}

AdStatus AdMediation::getInterstitialStatus() noexcept {
    AdStatus status{};
    
    for (auto& [bid, unit]: setup_.interstitialsByBid) {
        if (unit->loaded && !unit->showPending && !unit->shown) {
            status.ready = true;
            status.bid = bid;
            status.unitId = unit->unitId;
            break;
        }
    }

    return status;
}


void AdMediation::showAvailableInterstitial() {
    for (auto& [bid, unit]: setup_.interstitialsByBid) {
        if (unit->loaded && !unit->showPending) {
            requestInterstitialShow(unit);
            break;
        }
    }
}

void AdMediation::requestInterstitialLoad(AdUnitP unit) {
    unit->clearStatus();
    unit->loadPending = true;
   
    unit->network->interstitialRequestTimestamp = currentTimestamp_;
    sendRequestInterstitialLoad(unit->network->name, unit->unitId);
}


void AdMediation::requestInterstitialShow(AdUnitP unit) {
    unit->showPending = true;
    unit->showFailed = false;
    sendRequestInterstitialShow(unit->network->name, unit->unitId);
}


#endif
}  // namespace mangl

