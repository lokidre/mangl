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


void AdMediation::loadNetworksSetupData(StrView setupData, bool showException) {

    AdNetworksSetup setup;

    auto wasException = false;
    String exceptionMessage;
    auto appFound = false;

    try {
        using namespace apl;

        auto yaml = apl::parseYaml(setupData);

        for (auto& rootNode: yaml.front()) {
            switch (pshHash(rootNode.key)) {
            case "version"_psh:
                if (rootNode.value != "0.1")
                    return;
                break;
            case "apps"_psh:
                for (auto& appsNode: rootNode) {
                    for (auto& appNode: appsNode) {
                        switch (pshHash(appNode.key)) {
                            case "id"_psh: {
                                String appId;
                                appNode.get(&appId);
                                if (MANGL_APP_ID == appId) {
                                    appFound = true;
                                    loadNetworksSetupDataApp(appsNode, &setup.networks);
                                }
                                break;
                            }
                            default:
                                break;
                        }
                    }
                }
                break;
            default:
                break;
            }


        }


    }
    catch (const std::exception& e) {
        wasException = true;
        exceptionMessage = e.what();
    }
    catch (...) {
        wasException = true;
        exceptionMessage = "Unhandled Exception!";
    }

    if (wasException) {
        if (showException)
            mthrowError("Adm: Error: %s", exceptionMessage);
        return;
    }

    if (!appFound) {
        mthrowError("Adm: App not found %s:", MANGL_APP_ID);
    }

    //
    // Post-process
    //
    for (auto& network: setup.networks) {
        for (auto& unit: network.second->banners) {
            setup.bannersByBid.insert({unit.second->bid, unit.second.get()});
        }

        for (auto& unit: network.second->interstitials) {
            setup.interstitialsByBid.insert({unit.second->bid, unit.second.get()});
        }
        
        for (auto& unit: network.second->appopens) {
            setup.appopensByBid.insert({unit.second->bid, unit.second.get()});
        }

        
        for (auto& unit: network.second->rewarded) {
            setup.rewardedByBid.insert({unit.second->bid, unit.second.get()});
        }
    }
    
    if (setup_.shownBanner) {
        requestBannerHide(setup_.shownBanner);
    }
    for (auto& networkIt: setup_.networks) {
        requestNetworkReset(networkIt.second.get());
    }
    
    // Setup network initialization status
    for (auto& networkIt: setup.networks) {
        auto it = setup_.networks.find(networkIt.second->name);
        if (it != setup_.networks.end()) {
            networkIt.second->initialized = it->second->initialized;
        }
    }

    {
        std::swap(setup_, setup);
    }
}

void AdMediation::loadNetworksSetupDataApp(const apl::YamlNode& appsNode, AdNetworksContainer* networks) {
    using namespace apl;

    for (auto& appNode: appsNode) {
        switch (pshHash(appNode.key)) {
            case "networks"_psh: {
                for (auto& networksNode: appNode) {
                    String networkName;

                    for (auto& networkNode: networksNode) {
                        if (networkNode.key == "name") {
                            networkName = networkNode.value;
                            break;
                        }
                    }
                    
                    if (enabledNetworks_.find(networkName) == enabledNetworks_.end())
                        continue;
                    
                    
                    auto network = std::make_shared<AdNetwork>();
                    
                    for (auto& networkNode: networksNode) {
                        switch(pshHash(networkNode.key)) {
                            case "name"_psh:
                                networkNode.get(&network->name);
                                break;
                                
                            case "banners"_psh:
                                for (auto& unitNode: networkNode) {
                                    auto unit = std::make_shared<AdUnit>();
                                    unit->type = AdUnitType::Banner;
                                    unit->network = network.get();
                                    loadNetworksSetupDataAdUnit(unitNode, unit.get());
                                    apl::verify(!unit->unitId.empty(), "Banner Unit ID is missing: %s", network->name);
                                    apl::verify(!network->banners.contains(unit->unitId), "Duplicate banner unit id: %d", unit->unitId);
                                    network->banners.emplace(unit->unitId, std::move(unit));
                                }
                                break;
                                
                            case "interstitials"_psh:
                                for (auto& unitNode: networkNode) {
                                    auto unit = std::make_shared<AdUnit>();
                                    unit->type = AdUnitType::Interstitial;
                                    unit->network = network.get();
                                    loadNetworksSetupDataAdUnit(unitNode, unit.get());
                                    apl::verify(!unit->unitId.empty(), "Interstitial Unit ID is missing: %s", network->name);
                                    apl::verify(!network->interstitials.contains(unit->unitId), "Duplicate interstitial unit id: %d", unit->unitId);
                                    network->interstitials.emplace(unit->unitId, std::move(unit));
                                }
                                break;
                                
                            case "appopens"_psh:
                                for (auto& unitNode: networkNode) {
                                    auto unit = std::make_shared<AdUnit>();
                                    unit->type = AdUnitType::AppOpen;
                                    unit->network = network.get();
                                    loadNetworksSetupDataAdUnit(unitNode, unit.get());
                                    apl::verify(!unit->unitId.empty(), "App Open Unit ID is missing: %s", network->name);
                                    apl::verify(!network->appopens.contains(unit->unitId), "Duplicate app open unit id: %d", unit->unitId);
                                    network->appopens.emplace(unit->unitId, std::move(unit));
                                }
                                break;


                            case "rewarded"_psh:
                                for (auto& unitNode: networkNode) {
                                    auto unit = std::make_shared<AdUnit>();
                                    unit->type = AdUnitType::Rewarded;
                                    unit->network = network.get();
                                    loadNetworksSetupDataAdUnit(unitNode, unit.get());
                                    apl::verify(!unit->unitId.empty(), "Rewarded Unit ID is missing: %s", network->name);
                                    apl::verify(!network->rewarded.contains(unit->unitId), "Duplicate rewarded unit id: %d", unit->unitId);
                                    network->rewarded.emplace(unit->unitId, std::move(unit));
                                }
                                break;

                            default:
                                break;
                        }
                    }
                    
                    networks->emplace(network->name, std::move(network));
                }
                break;
            }

        case "id"_psh:
            break;

        default:
            break;
        }
    }
}


void AdMediation::loadNetworksSetupDataAdUnit(const apl::YamlNode& unitNode, AdUnitP unit) 
{
    using namespace apl;
    
    unit->refresh = 15;

    for (auto& node: unitNode) {
        switch (pshHash(node.key)) {
        case "id"_psh:   node.get(&unit->unitId); break;
        case "bid"_psh:  node.get(&unit->bid); break;
        case "refresh"_psh: node.get(&unit->refresh); break;
        default:
            break;
        }
    }
    

}


#endif

}  // namespace mangl
