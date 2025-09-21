#import "mangl_ad_network.h"

#if MANGL_ADS

#include "../../core/ads/mangl_ad_mediation.h"
#include "../../core/mangl_ads.h"
#include "../../core/mangl_framework.h"
#include "../../core/audio/mangl_game_music.h"
#include "../../core/mangl_resources.h"


#include "mangl_ad_adaptor.h"

#if MANGL_ADS_ADMOB
#import "networks/mangl_ad_adaptor_admob.h"
#endif

#if MANGL_ADS_APPLOVIN
#import "networks/mangl_ad_adaptor_applovin.h"
#endif

#if __has_include(<FBAudienceNetwork/FBAudienceNetwork.h>)
#import <FBAudienceNetwork/FBAudienceNetwork.h>
#define MANGL_HAVE_FACEBOOK 1
#endif


using namespace mangl;


@interface ManglAdNetwork() {
    bool initialized_;
    
    bool pendingBannerShow_;
    MString pendingBannerNetwork_;
    MString pendingBannerUnitId_;
    
@public
    bool appOpenPendingShow_;

@private
    bool appOpenLoaded_;
//    bool appOpenShowing_;
    
    
    bool pendingRewardedShow_;
    MString pendingRewardedNetwork_;
    MString pendingRewardedUnitId_;
    
    AdMediation* mediation_;

    std::vector<String> enabledNetworks_;
}

@property (nonatomic) NSMutableDictionary<NSString*, ManglAdAdaptor*> *adaptors;

@end



namespace mangl {

static StrView getAdActiveNetworkName() {
#if MANGL_ADS_ADMOB
    return ManglAdNetworkNameAdmob;
#endif
#if MANGL_ADS_APPLOVIN
    return ManglAdNetworkNameAppLovin;
#endif

    MDebugTrap();
    return {};
}

static StrView getAdActiveBannerUnitId() {
#if defined(MANGL_ADS_ADMOB) && defined(MANGL_ADS_BANNER_ADMOB_UNIT_ID)
    return MANGL_ADS_BANNER_ADMOB_UNIT_ID;
#endif
#if defined(MANGL_ADS_APPLOVIN) && defined(MANGL_ADS_BANNER_APPLOVIN_UNIT_ID)
    return MANGL_ADS_BANNER_APPLOVIN_UNIT_ID;
#endif
    MDebugTrap();
    return {};
}

static StrView getAdActiveInterstitialUnitId() {
#if defined(MANGL_ADS_ADMOB) && defined(MANGL_ADS_INTERSTITIAL_ADMOB_UNIT_ID)
    return MANGL_ADS_INTERSTITIAL_ADMOB_UNIT_ID;
#endif
    
#if defined(MANGL_ADS_APPLOVIN) && defined(MANGL_ADS_INTERSTITIAL_APPLOVIN_UNIT_ID)
    return MANGL_ADS_INTERSTITIAL_APPLOVIN_UNIT_ID;
#endif
//    MDebugTrap();
    return {};
}

static StrView getAdActiveAppOpenUnitId() {
#if defined(MANGL_ADS_ADMOB) && defined(MANGL_ADS_APPOPEN_ADMOB_UNIT_ID)
    return MANGL_ADS_APPOPEN_ADMOB_UNIT_ID;
#endif
    
#if defined(MANGL_ADS_APPLOVIN) && defined(MANGL_ADS_APPOPEN_APPLOVIN_UNIT_ID)
    return MANGL_ADS_APPOPEN_APPLOVIN_UNIT_ID;
#endif
//    MDebugTrap();
    return {};
}


static StrView getAdActiveRewardedUnitId() {
#if defined(MANGL_ADS_REWARDED_ADMOB_UNIT_ID)
    return MANGL_ADS_REWARDED_ADMOB_UNIT_ID;
#endif
#if defined(MANGL_ADS_REWARDED_APPLOVIN_UNIT_ID)
    return MANGL_ADS_REWARDED_APPLOVIN_UNIT_ID;
#endif
//    MDebugTrap();
    return {};
}


} // namespace mangl


@implementation ManglAdNetwork


// MARK: Initialization

+ (instancetype)instance {
	static ManglAdNetwork* shared = nil;
	static dispatch_once_t onceToken;
	dispatch_once(&onceToken, ^{
		shared = [[self alloc] init];
	});
	return shared;
}


- (instancetype)init {
    self = [super init];
    
    _rootViewController = nil;
    
    mediation_ = AdMediation::instance();

    initialized_ = false;
    pendingBannerShow_ = false;
    
    appOpenPendingShow_ = false;
    appOpenLoaded_ = false;
    
    pendingRewardedShow_ = false;
    
    enabledNetworks_.clear();
    
    _adaptors = [[NSMutableDictionary<NSString*, ManglAdAdaptor*> alloc] init];
    
    
#if MANGL_ADS_ADMOB
    [self initAddAdaptor:[[ManglAdAdaptorAdMob alloc] init]];
#endif
    
#if MANGL_ADS_APPLOVIN
    [self initAddAdaptor:[[ManglAdAdaptorAppLovin alloc] init]];
#endif

#if MANGL_ADS_MEDIATION
    mediation_->setEnabledNetworks(enabledNetworks_);
    auto setupDataFile = Resources::bundleDirJoin(MANGL_ADS_MEDIATION_SETUP_FILE);
    auto setupData = apl::loadFile(setupDataFile);
    mediation_->loadSetup(setupData, true);
#endif
    
	return self;
}

- (void)onViewDidLoad {
    _rootViewController = ManglMainViewController.instance;
    
}


- (void)initAddAdaptor:(ManglAdAdaptor *)adaptor {
    _adaptors[toNSString(adaptor->name)] = adaptor;
    enabledNetworks_.push_back(adaptor->name);
}


- (void)onInitNetwork {
#if MANGL_HAVE_FACEBOOK
    // Facebook initialization routine
    [FBAdSettings setAdvertiserTrackingEnabled:YES];
#endif
    
    for (id key in _adaptors) {
        ManglAdAdaptor* adaptor = [_adaptors objectForKey:key];
        [adaptor onInitNetwork];
    }

    AdMediation::instance()->onApplicationLaunch();
}

- (void)onAdaptorInitialized:(ManglAdAdaptor *)adaptor {
    adaptor->initialized_ = true;
    
    mediation_->onNetworkInitialized(adaptor->name);

    
    // Check if all adaptors are initialized
    bool adaptorsInitialized = true;
    for (id key in _adaptors) {
        ManglAdAdaptor* adaptor = [_adaptors objectForKey:key];
        if (!adaptor->initialized_) {
            adaptorsInitialized = false;
            break;
        }
    }
    
    if (adaptorsInitialized) {
        dispatch_async(dispatch_get_main_queue(), ^{
            [self processInitCompletion];
        });
    }
}

- (void)processInitCompletion {
    initialized_ = true;
    
    if (_rootViewController == nil)
        _rootViewController = ManglMainViewController.instance;

    Ads::bannerLayoutChangedCallback = ^{
//        [self moveBanner:Ads::bannerLayout()];
        for (id key in self.adaptors) {
            ManglAdAdaptor* adaptor = [self.adaptors objectForKey:key];
            // TODO: change banner layout notification
//            [adaptor setBannerLayout:Ads::bannerLayout() parentView:adaptor.currentParentView];
        }
    };

    for (id key in _adaptors) {
        ManglAdAdaptor* adaptor = [_adaptors objectForKey:key];
        adaptor.rootViewController = _rootViewController;
        adaptor.currentParentView = _rootViewController.view;
//        [adaptor setBannerLayout:Ads::bannerLayout() parentView:adaptor.currentParentView];
    }

    if (pendingBannerShow_) {
        pendingBannerShow_ = false;
        [self requestBannerShow:pendingBannerNetwork_ unitId:pendingBannerUnitId_];
    }

    // Run every 10 minutes
    int scheduleMinutes = 10;
    [NSTimer scheduledTimerWithTimeInterval:60 * scheduleMinutes
                                     target:self
                                   selector:@selector(maintenanceProcedure:)
                                   userInfo:nil
                                    repeats:YES];

    
#if !defined(MANGL_ADS_MEDIATION)
    
    // Request interstitial load if there is no mediation
    auto interstitialUnitId = getAdActiveInterstitialUnitId();
    if (!interstitialUnitId.empty()) {
        [self requestInterstitialLoad:getAdActiveNetworkName() unitId:interstitialUnitId];
    }
    
    // Request app open load if there is no mediation
    auto appOpenUnitId = getAdActiveAppOpenUnitId();
    if (!appOpenUnitId.empty()) {
        [self requestAppOpenLoad:getAdActiveNetworkName() unitId:appOpenUnitId];
    }
#endif
}


- (void)onApplicationDidBecomeActive {
    [self maintenanceProcedure:nil];
}

- (void)onApplicationWillResignActive {
}


//// MARK: Adaptor utilities
//
//- (ManglAdAdaptor *)getActiveAdaptor {
//    for (id key in _adaptors) {
//        ManglAdAdaptor* adaptor = [_adaptors objectForKey:key];
//#if MANGL_ADS_ADMOB
//        if (adaptor->name == ManglAdNetworkNameAdmob) return adaptor;
//#endif
//    }
//
//    return nil;
//}

// MARK: Utilities
- (CGRect)getBannerRectCG {
    auto bannerRect = Ads::bannerLayout();
    auto screenRect = Env::toScreen(bannerRect);
    auto cgRect = toCGRect(screenRect);
    return cgRect;
}

- (void)maintenanceProcedure:(NSTimer *)timer {
    for (id key in _adaptors) {
        ManglAdAdaptor* adaptor = [_adaptors objectForKey:key];
        [adaptor maintentanceFullScreenAds];
    }
}

- (void)processShowFullScreen {
    auto fw = Framework::instance();
    fw->onInterstitialWillPresentFullScreen();
    fw->onPause();

    GameMusic::processAdWillPresentFullScreen();
}

- (void)processDismissFullScreen {
    auto fw = Framework::instance();
    fw->onAdsDidDismissFullScreen();
    
    GameMusic::processAdDidDismissFullScreen();
}


- (void)requestNetworkReset:(MStrView)network {
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
    [adaptor requestNetworkReset];
}



// MARK: Banners
- (void)requestBannerLoad:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
    [adaptor requestBannerLoad:unitId];
}


- (void)requestBannerShow:(MStrView)network unitId:(MStrView)unitId {
    if (!initialized_) {
        pendingBannerShow_ = true;
        pendingBannerNetwork_ = network;
        pendingBannerUnitId_ = unitId;
        return;
    }
    
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
//    NSString* network = toNSString(networkName);
//    NSString* unitId = toNSString(aUnitId);
    
    // Walk through all the adaptors and hide other adaptors code
    
    for (id key in _adaptors) {
        
        ManglAdAdaptor* adaptor = [_adaptors objectForKey:key];

        if (adaptor->name == network) {
            [adaptor requestBannerShow:unitId];
        } else {
            [adaptor requestBannerRemove];
        }
        
    }
}


- (void)requestBannerRemove:(MStrView)network {
    for (id key in _adaptors) {
        ManglAdAdaptor* adaptor = [_adaptors objectForKey:key];
        if (adaptor->name == network) {
            [adaptor requestBannerRemove];
        }
    }
}


- (void)requestBannerRemoveAll {
    for (id key in _adaptors) {
        ManglAdAdaptor* adaptor = [_adaptors objectForKey:key];
        [adaptor requestBannerRemove];
    }
}

//
// MARK: Interstitials
//
- (bool)isInterstitialReady:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return false;
    
    return [adaptor isInterstitialReady:unitId];
}

- (void)requestInterstitialLoad:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
    [adaptor requestInterstitialLoad:unitId];
}

- (void)requestInterstitialShow:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
    [adaptor requestInterstitialShow:unitId];
}

//
// MARK: AppOpen
//
- (bool)isAppOpenReady:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return false;
    
    return [adaptor isAppOpenReady:unitId];
}

- (void)requestAppOpenLoad:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
    [adaptor requestAppOpenLoad:unitId];
}

- (void)requestAppOpenShow:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
#ifndef MANGL_ADS_MEDIATION
    if (!appOpenLoaded_) {
        if (!appOpenPendingShow_) {
            appOpenPendingShow_ = true;
            [self requestAppOpenLoad:network unitId:unitId];
        }
    }
#endif
    
    [adaptor requestAppOpenShow:unitId];
}


- (void)requestAppOpenDismiss:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
    
    [adaptor requestAppOpenDismiss:unitId];
}


//
// MARK: Rewarded
//
- (void)requestRewardedLoad:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
    [adaptor requestRewardedLoad:unitId];
}

- (void)requestRewardedShow:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return;
    
    if (![adaptor isRewardedReady:unitId]) {
        pendingRewardedShow_ = true;
        pendingRewardedNetwork_ = network;
        pendingRewardedUnitId_ = unitId;
        
        [self requestRewardedLoad:network unitId:unitId];
        return;
    }
    
    [adaptor requestRewardedShow:unitId];
}

- (bool)isRewardedReady:(MStrView)network unitId:(MStrView)unitId {
    MDebugFunc("Network: %s, unitId: %s", network, unitId);
    
    id adaptor = [_adaptors objectForKey:toNSString(network)];
    if (!adaptor) return false;
    
    return [adaptor isRewardedReady:unitId];
}




// MARK: Handling events

// MARK: Banners
- (void)onBannerLoaded:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        AdMediation::instance()->onBannerLoaded(adaptor->name, toStringView(unitId));
    });
}

- (void)onBannerLoadError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto errorMsg = toStringView(error.description);
        AdMediation::instance()->onBannerLoadError(adaptor->name, toStringView(unitId), errorMsg);
    });

}

- (void)onBannerDidShow:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
}

- (void)onBannerClicked:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        AdMediation::instance()->onBannerClicked(adaptor->name, toStringView(unitId));
    });
}


- (void)onBannerShowError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto errorMsg = toStringView(error.description);
        AdMediation::instance()->onBannerShowError(adaptor->name, toStringView(unitId), errorMsg);
    });
}

- (void)onBannerWillShowFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self processShowFullScreen];
    });
}

- (void)onBannerDidDismissFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self processDismissFullScreen];
    });
}


//
// MARK: Interstitials
//
- (void)onInterstitialLoaded:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        AdMediation::instance()->onInterstitialLoaded(adaptor->name, toStringView(unitId));
    });
}

- (void)onInterstitialLoadError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto errorMsg = toStringView(error.description);
        AdMediation::instance()->onInterstitialLoadError(adaptor->name, toStringView(unitId), errorMsg);
    });
}

- (void)onInterstitialShowError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto errorMsg = toStringView(error.description);
        AdMediation::instance()->onInterstitialShowError(adaptor->name, toStringView(unitId), errorMsg);
    });
}

- (void)onInterstitialWillPresentFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto sUnitId = toStringView(unitId);
        AdMediation::instance()->onInterstitialWillPresentFullScreen(adaptor->name, sUnitId);
        [self processShowFullScreen];
        if (Ads::onInterstitialShown) Ads::onInterstitialShown(sUnitId);
    });
}

- (void)onInterstitialDidDismissFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto sUnitId = toStringView(unitId);
        AdMediation::instance()->onInterstitialDidDismissFullScreen(adaptor->name, sUnitId);
        [self processDismissFullScreen];
        
#if !defined(MANGL_ADS_MEDIATION)
        [self requestInterstitialLoad:getAdActiveNetworkName() unitId:getAdActiveInterstitialUnitId()];
#endif
        if (Ads::onInterstitialDismissed) Ads::onInterstitialDismissed(sUnitId);
    });
}


//
// MARK: AppOpen
//

- (void)onAppOpenLoaded:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        AdMediation::instance()->onAppOpenLoaded(adaptor->name, toStringView(unitId));
    });
    
#if !defined(MANGL_ADS_MEDIATION)
    appOpenLoaded_ = true;
    if (appOpenPendingShow_) {
        [self requestAppOpenShow:getAdActiveNetworkName() unitId:toStringView(unitId)];
        appOpenPendingShow_ = false;
    }
#endif

}

- (void)onAppOpenLoadError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto errorMsg = toStringView(error.description);
        AdMediation::instance()->onAppOpenLoadError(adaptor->name, toStringView(unitId), errorMsg);
    });
}

- (void)onAppOpenShowError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto errorMsg = toStringView(error.description);
        AdMediation::instance()->onAppOpenShowError(adaptor->name, toStringView(unitId), errorMsg);
    });
}

- (void)onAppOpenWillPresentFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto sUnitId = toStringView(unitId);
        AdMediation::instance()->onAppOpenWillPresentFullScreen(adaptor->name, sUnitId);
        [self processShowFullScreen];
        if (Ads::onAppOpenShown) Ads::onAppOpenShown(sUnitId);
    });
    
#if !defined(MANGL_ADS_MEDIATION)
    appOpenPendingShow_ = false;
#endif
}

- (void)onAppOpenDidDismissFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto sUnitId = toStringView(unitId);
        AdMediation::instance()->onAppOpenDidDismissFullScreen(adaptor->name, sUnitId);
        [self processDismissFullScreen];
        
#if !defined(MANGL_ADS_MEDIATION)
        [self requestAppOpenLoad:getAdActiveNetworkName() unitId:getAdActiveAppOpenUnitId()];
#endif
        if (Ads::onAppOpenDismissed) Ads::onAppOpenDismissed(sUnitId);
    });
}



//
// MARK: Rewarded
//

- (void)onRewardedLoaded:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        AdMediation::instance()->onRewardedLoaded(adaptor->name, toStringView(unitId));
        
        if (self->pendingRewardedShow_) {
            auto sUnitId = toStringView(unitId);
            if (adaptor->name == self->pendingRewardedNetwork_ && self->pendingRewardedUnitId_ == sUnitId) {
                self->pendingRewardedShow_ = false;
                [self requestRewardedShow:adaptor->name unitId:sUnitId];
            }
        }
    });
}

- (void)onRewardedLoadError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto errorMsg = toStringView(error.description);
        AdMediation::instance()->onRewardedLoadError(adaptor->name, toStringView(unitId), errorMsg);
        if (Ads::onRewardedError) Ads::onRewardedError(errorMsg);
    });
}

- (void)onRewardedShowError:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId withError:(NSError *)error {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto errorMsg = toStringView(error.description);
        AdMediation::instance()->onRewardedShowError(adaptor->name, toStringView(unitId), errorMsg);
        if (Ads::onRewardedError) Ads::onRewardedError(errorMsg);
    });
}

- (void)onRewardedWillPresentFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self processShowFullScreen];
    });
}

- (void)onRewardedDidDismissFullScreen:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId {
    dispatch_async(dispatch_get_main_queue(), ^{
        [self processDismissFullScreen];
    });
}

- (void)onRewardEarned:(ManglAdAdaptor *)adaptor unitId:(NSString *)unitId reward:(NSString *)reward amount:(double)amount {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto sReward = toStringView(reward);
        AdMediation::instance()->onRewardEarned(adaptor->name, toStringView(unitId), sReward, amount);
        if (Ads::onRewardEarned) Ads::onRewardEarned(sReward, amount);
    });
}


@end



namespace mangl {


#if MANGL_ADS_MEDIATION

void AdMediation::sendRequestNetworkReset(StrView networkName) {
    [ManglAdNetwork.instance requestNetworkReset:networkName];
}


void AdMediation::requestBannerLoad(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestBannerLoad:networkName unitId:unitId];
}

void AdMediation::requestBannerShow(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestBannerShow:networkName unitId:unitId];
}


void AdMediation::requestBannerHide(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestBannerRemove:networkName];
}


//
// Interstitials
//
void AdMediation::sendRequestInterstitialLoad(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestInterstitialLoad:networkName unitId:unitId];
}

void AdMediation::sendRequestInterstitialShow(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestInterstitialShow:networkName unitId:unitId];
}

//
// AppOpen
//
void AdMediation::sendRequestAppOpenLoad(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestAppOpenLoad:networkName unitId:unitId];
}

void AdMediation::sendRequestAppOpenShow(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestAppOpenShow:networkName unitId:unitId];
}

void AdMediation::sendRequestAppOpenDismiss(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestAppOpenDismiss:networkName unitId:unitId];
}



//
// Rewarded
//
void AdMediation::sendRequestRewardedLoad(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestRewardedLoad:networkName unitId:unitId];
}

void AdMediation::sendRequestRewardedShow(StrView networkName, StrView unitId) {
    [ManglAdNetwork.instance requestRewardedShow:networkName unitId:unitId];
}


#else // No mediation

void Ads::showBanner() {
    if (!Ads::enabled()) return;
    
    Ads::bannerHidden = false;
    [ManglAdNetwork.instance requestBannerShow:getAdActiveNetworkName()
                                        unitId:getAdActiveBannerUnitId()];
}

void Ads::hideBanner() {
    Ads::bannerHidden = true;
    [ManglAdNetwork.instance requestBannerRemoveAll];
}

AdStatus Ads::getInterstitialStatus() noexcept {
    AdStatus status;
    status.ready = Ads::isRewardedReady();
    status.bid = 0;
    status.unitId = getAdActiveInterstitialUnitId();
    return status;
}

bool Ads::isInterstitialReady() noexcept {
    return [ManglAdNetwork.instance isInterstitialReady:getAdActiveNetworkName()
                                                 unitId:getAdActiveInterstitialUnitId()];
}



void Ads::showInterstitial() {
    if (!Ads::enabled()) return;
    
    [ManglAdNetwork.instance requestInterstitialShow:getAdActiveNetworkName()
                                              unitId:getAdActiveInterstitialUnitId()];
}

void Ads::showInterstitialDelayed(double delay) {
    if (!Ads::enabled()) return;
    
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delay * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(){
        [ManglAdNetwork.instance requestInterstitialShow:getAdActiveNetworkName()
                                                  unitId:getAdActiveInterstitialUnitId()];
    });
}


//
// MARK: AppOpen
//
void Ads::showAppOpen(){
    if (!Ads::enabled()) return;
    
    [ManglAdNetwork.instance requestAppOpenShow:getAdActiveNetworkName()
                                         unitId:getAdActiveAppOpenUnitId()];
}

void Ads::scheduleAppOpen() {
    if (!Ads::enabled()) return;
    
    if ([ManglAdNetwork.instance isAppOpenReady:getAdActiveNetworkName() unitId:getAdActiveAppOpenUnitId()]) {
        [ManglAdNetwork.instance requestAppOpenShow:getAdActiveNetworkName()
                                             unitId:getAdActiveAppOpenUnitId()];
    } else {
        ManglAdNetwork.instance->appOpenPendingShow_ = true;
    }
}

void Ads::dismissAppOpen() {
    ManglAdNetwork.instance->appOpenPendingShow_ = false;
}


//
// MARK: Rewarded
//

void Ads::showRewarded() {
    [ManglAdNetwork.instance requestRewardedShow:getAdActiveNetworkName()
                                          unitId:getAdActiveRewardedUnitId()];
}

bool Ads::isRewardedReady() noexcept {
    return [ManglAdNetwork.instance isRewardedReady:getAdActiveNetworkName()
                                             unitId:getAdActiveRewardedUnitId()];
}

#endif

}  // namespace mangl

#endif
