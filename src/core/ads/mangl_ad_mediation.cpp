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

AdNetworkP AdMediation::findNetwork(StrView networkName) {
    auto networkIt = setup_.networks.find(String{networkName});
    if (networkIt == setup_.networks.end())
        return nullptr;
    
    auto network = networkIt->second.get();

    return network;
}

AdUnitP AdMediation::findBannerUnit(StrView networkName, StrView unitId) {

    auto network = findNetwork(networkName);
    if (!network)
        return nullptr;
    
    auto unitIt = network->banners.find(String{unitId});
    if (unitIt != network->banners.end())
        return unitIt->second.get();
    
    return nullptr;
}

AdUnitP AdMediation::findInterstitialUnit(StrView networkName, StrView unitId) {

    auto network = findNetwork(networkName);
    if (!network)
        return nullptr;
    
    auto unitIt = network->interstitials.find(String{unitId});
    if (unitIt != network->interstitials.end())
        return unitIt->second.get();

    return nullptr;
}

AdUnitP AdMediation::findAppOpenUnit(StrView networkName, StrView unitId) {

    auto network = findNetwork(networkName);
    if (!network)
        return nullptr;
    
    auto unitIt = network->appopens.find(String{unitId});
    if (unitIt != network->appopens.end())
        return unitIt->second.get();

    return nullptr;
}


AdUnitP AdMediation::findRewardedUnit(StrView networkName, StrView unitId) {

    auto network = findNetwork(networkName);
    if (!network)
        return nullptr;
    
    auto unitIt = network->rewarded.find(String{unitId});
    if (unitIt != network->rewarded.end())
        return unitIt->second.get();

    return nullptr;
}



AdUnitP AdMediation::findUnit(StrView networkName, StrView unitId) {
    
    auto unit = findBannerUnit(networkName, unitId);
    if (!unit)
        unit = findInterstitialUnit(networkName, unitId);
    if (!unit)
        unit = findAppOpenUnit(networkName, unitId);
    return unit;
}

#endif

}  // namespace mangl

