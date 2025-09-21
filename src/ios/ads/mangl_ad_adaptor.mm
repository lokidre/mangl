#import "mangl_ad_adaptor.h"


#if MANGL_ADS

//#if MANGL_ADS_MEDIATION

#include "../../core/ads/mangl_ad_mediation.h"

#import "../mangl_main_vc.h"
#import "../mangl_main_app.h"
#import "mangl_ad_network.h"

namespace mangl {

struct ManglFullScreenAdContext {
    ManglAdType type{};
    std::chrono::time_point<std::chrono::system_clock> loadedTime{};
    bool loaded{};
    bool ready{};
    bool displayed{};
};

}  // namespace mangl


using namespace mangl;

@interface ManglAdAdaptor() {
    std::map<String, ManglFullScreenAdContext> adContexts_;
}
@end

@implementation ManglAdAdaptor

// MARK: Initialization
- (instancetype)init {
	self = [super init];
    
    initialized_ = false;

    return self;
}

- (void)onInitNetwork {
    MDebugFunc("Warning: Not overridden");
}

- (bool)isInitialized {
    return initialized_;
}


- (void)processAdaptorInitCompletion {
    initialized_ = true;
    [ManglAdNetwork.instance onAdaptorInitialized:self];
}

// MARK: C++ interfacing
- (void)requestNetworkReset {
    MDebugTrap();
}

// MARK: Utilities
- (void)recordFullScreenAdLoad:(NSString*)unitId type:(mangl::ManglAdType)type{
    auto& ctx = self->adContexts_[toString(unitId)];
    ctx.type = type;
    ctx.loaded = true;
    ctx.ready = true;
    ctx.displayed = false;
    ctx.loadedTime = std::chrono::system_clock::now();
}


- (void)recordFullScreenAdDisplay:(NSString*)unitId type:(mangl::ManglAdType)type {
    auto& ctx = self->adContexts_[toString(unitId)];
    ctx.type = type;
    ctx.loaded = true;
    ctx.ready = false;
    ctx.displayed = true;
    ctx.loadedTime = std::chrono::system_clock::now();
}

- (void)recordFullScreenAdError:(NSString*)unitId type:(mangl::ManglAdType)type {
    auto& ctx = self->adContexts_[toString(unitId)];
    ctx.type = type;
    ctx.loaded = false;
    ctx.ready = false;
    ctx.displayed = true;
    ctx.loadedTime = std::chrono::system_clock::now();
}

- (void)maintentanceFullScreenAds {
    auto now = std::chrono::system_clock::now();
    constexpr int adTimeoutMinutes = 50;
    
    for (auto& [unitId, ctx]: self->adContexts_) {
        if (!ctx.loaded) continue;
        if (ctx.displayed) continue;
        if (!ctx.ready) continue;
        
        auto minutes = std::chrono::duration_cast<std::chrono::minutes>(now - ctx.loadedTime);
        
        if (minutes.count() > adTimeoutMinutes) {
            switch (ctx.type) {
                case ManglAdType::Interstitial:
                    [self requestInterstitialLoad:unitId];
                    break;
                case ManglAdType::Rewarded:
                    [self requestRewardedLoad:unitId];
                    break;
                default:
                    break;
            }
        }
    }
}



- (void)requestBannerLoad:(MStrView)unitId {
    MDebugTrap();
}

- (void)requestBannerShow:(MStrView)unitId {
    MDebugTrap();
}

- (void)requestBannerRemove {
    MDebugTrap();
}

- (bool)isInterstitialReady:(MStrView)unitId {
    MDebugTrap();
    return false;
}



//
// Interstitials
//
- (void)requestInterstitialLoad:(MStrView)unitId {
    MDebugTrap();
}
- (void)requestInterstitialShow:(MStrView)unitId {
    MDebugTrap();
}

- (void)requestAppOpenDismiss:(MStrView)unitId {
    MDebugTrap();
}


//
// App Open
//
- (bool)isAppOpenReady:(MStrView)unitId {
    MDebugTrap();
    return false;
}

- (void)requestAppOpenLoad:(MStrView)unitId {
    MDebugTrap();
}

- (void)requestAppOpenShow:(MStrView)unitId {
    MDebugTrap();
}



//
// MARK: Rewarded
//
- (bool)isRewardedReady:(MStrView)unitId {
    MDebugTrap();
    return false;
}

- (void)requestRewardedLoad:(MStrView)unitId {
    MDebugTrap();
}

- (void)requestRewardedShow:(MStrView)unitId {
    MDebugTrap();
}



@end

#endif // if ads mediation


