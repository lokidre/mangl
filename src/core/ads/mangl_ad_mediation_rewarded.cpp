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

bool AdMediation::isRewardedReady() noexcept {
    for (auto& [bid, unit]: setup_.rewardedByBid) {
        if (unit->loaded && !unit->showPending && !unit->shown)
            return true;
    }
    
    return false;
}


void AdMediation::showRewarded() {
    for (auto& [bid, unit]: setup_.rewardedByBid) {
        if (unit->loaded && !unit->showPending && !unit->shown) {
            MDebugPrint("ADM: Rewarded request show: (%g) %s - %s", unit->bid, unit->network->name, unit->unitId);
            requestRewardedShow(unit);
            return;
        }
    }

    // Now loaded rewarded ad found, request load
    rewardedShowRequested_ = true;
    for (auto& [bid, unit]: setup_.rewardedByBid) {
        if (!unit->network->initialized) continue;
        MDebugPrint("ADM: Rewarded request load: (%g) %s - %s", unit->bid, unit->network->name, unit->unitId);
        requestRewardedLoad(unit);
    }
}


void AdMediation::requestRewardedLoad(AdUnitP unit) {
    unit->clearStatus();
    unit->loadPending = true;
    sendRequestRewardedLoad(unit->network->name, unit->unitId);
}


void AdMediation::requestRewardedShow(AdUnitP unit) {
    unit->showPending = true;
    unit->showFailed = false;
    sendRequestRewardedShow(unit->network->name, unit->unitId);
}


#endif

}  // namespace mangl

