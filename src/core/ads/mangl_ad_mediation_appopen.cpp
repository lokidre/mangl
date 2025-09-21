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

bool AdMediation::isAppOpenReady() noexcept {
    for (auto& [bid, unit]: setup_.appopensByBid) {
        if (unit->loaded && !unit->showPending && !unit->shown)
            return true;
    }
    
    return false;
}

AdStatus AdMediation::getAppOpenStatus() noexcept {
    AdStatus status;
    
    for (auto& [bid, unit]: setup_.appopensByBid) {
        if (unit->loaded && !unit->showPending && !unit->shown) {
            status.ready = true;
            status.bid = bid;
            status.unitId = unit->unitId;
            break;
        }
    }

    return status;
}


bool AdMediation::showAvailableAppOpen() {
    if (appOpenShowScheduled_) return true;
    if (appOpenShowing_) return true;

    if (appOpenShowPending_) return true;

    for (auto& [bid, unit]: setup_.appopensByBid) {
        if (unit->loaded && !unit->showPending) {
            requestAppOpenShow(unit);
            return true;
        }
    }
    
    return false;
}

bool AdMediation::scheduleAppOpenShow() {
    if (showAvailableAppOpen()) return true;
    
    appOpenShowScheduled_ = true;
    appOpenShowPending_ = true;

    return false;
}

bool AdMediation::dismissAppOpen() {
    appOpenShowScheduled_ = false;
    appOpenShowPending_ = false;
    
    if (appOpenShowing_) {
        
        return true;
    }
    
    return false;
}


void AdMediation::requestAppOpenLoad(AdUnitP unit) {
    unit->clearStatus();
    unit->loadPending = true;
   
    unit->network->appOpenRequestTimestamp = currentTimestamp_;
    sendRequestAppOpenLoad(unit->network->name, unit->unitId);
}


void AdMediation::requestAppOpenShow(AdUnitP unit) {
    unit->showPending = true;
    unit->showFailed = false;
    sendRequestAppOpenShow(unit->network->name, unit->unitId);
}


#endif
}  // namespace mangl


