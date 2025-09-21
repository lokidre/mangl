#import "mangl_ad_adaptor_applovin.h"
#import "../mangl_ad_network.h"


#if MANGL_ADS_APPLOVIN || MANGL_ADS_MEDIATION_APPLOVIN

using namespace mangl;

struct AppLovinInterstitialStatus {
    bool shown{};
};

@interface ManglAdAdaptorAppLovin() {
    bool bannerShowPending_;
    std::map<String, AppLovinInterstitialStatus> interstitialStatuses_;
    std::map<String, AppLovinInterstitialStatus> appopenStatuses_;
}

@property (nonatomic) NSMutableDictionary<NSString*, MAAdView*>* bannerViews;
@property (nonatomic, weak) MAAdView* currentBannerView;


@property (nonatomic) NSMutableDictionary<NSString*, MAInterstitialAd*>* interstitials;
@property (nonatomic) NSMutableDictionary<NSString*, MAAppOpenAd*>* appopens;

@end


@implementation ManglAdAdaptorAppLovin

- (id)init {
    self = [super init];
    
    self->name = ManglAdNetworkNameAppLovin;
    
    bannerShowPending_ = false;
    _currentBannerView = nil;

    _bannerViews = [[NSMutableDictionary<NSString*, MAAdView*> alloc] init];
    _interstitials = [[NSMutableDictionary<NSString*, MAInterstitialAd*> alloc] init];
    _appopens = [[NSMutableDictionary<NSString*, MAAppOpenAd*> alloc] init];

    return self;
}

- (void)requestNetworkReset {
    if (_currentBannerView) {
        _currentBannerView.hidden = true;
        [_currentBannerView removeFromSuperview];
        _currentBannerView = nil;
    }
    
   
    [_bannerViews removeAllObjects];
    
    [_interstitials removeAllObjects];
    interstitialStatuses_.clear();
    
    [_appopens removeAllObjects];
    appopenStatuses_.clear();
}



- (void)onInitNetwork {
//    ALSdk.shared.mediationProvider = @"max";
//      
//    ALSdk.shared.userIdentifier = @"USER_ID";
//
//#if defined(_DEBUG) && defined(MANGL_ADS_APPLOVIN_MEDIATION_DEBUGGER)
//    ALSdk.shared.settings.verboseLoggingEnabled = YES;
//#endif
//      
//    [ALSdk.shared initializeSdkWithCompletionHandler:^(ALSdkConfiguration *configuration) {
//        
//#if defined(_DEBUG) && defined(MANGL_ADS_APPLOVIN_MEDIATION_DEBUGGER)
//        [ALSdk.shared showMediationDebugger];
//#endif
//      
//        self->isInitialized_ = true;
//        [super processAdaptorInitCompletion];
//    }];
    
    
    // Create the initialization configuration
    ALSdkInitializationConfiguration *initConfig = [ALSdkInitializationConfiguration configurationWithSdkKey: @MANGL_ADS_APPLOVIN_SDK_KEY
                                                                                                builderBlock:^(ALSdkInitializationConfigurationBuilder *builder) {
        builder.mediationProvider = ALMediationProviderMAX;
        
      // Perform any additional configuration/setting changes
#ifdef MANGL_ADS_APPLOVIN_TEST_DEVICE_ID
        builder.testDeviceAdvertisingIdentifiers = @[@MANGL_ADS_APPLOVIN_TEST_DEVICE_ID];
#endif
    }];
    
    // Initialize the SDK with the configuration
    [ALSdk.shared initializeWithConfiguration:initConfig completionHandler:^(ALSdkConfiguration *sdkConfig) {
        
        ALSdk.shared.settings.muted = YES;

        [super processAdaptorInitCompletion];
    }];
    
}


// MARK: banners

- (void)requestBannerLoad:(mangl::StrView)unitIdView {
    auto unitId = toNSString(unitIdView);

    MAAdView* bannerView = [_bannerViews objectForKey:unitId];

    if (!bannerView) {
        bannerView = [[MAAdView alloc] initWithAdUnitIdentifier:unitId];
        bannerView.delegate = self;
        bannerView.backgroundColor = UIColor.blackColor;
        
#if MANGL_ADS_MEDIATION
        [bannerView setExtraParameterForKey: @"allow_pause_auto_refresh_immediately" value: @"true"];
        [bannerView stopAutoRefresh];
#endif

        _bannerViews[unitId] = bannerView;
    }
    
    [bannerView loadAd];
}

- (void)requestBannerShow:(mangl::StrView)unitIdView {
    auto unitId = toNSString(unitIdView);

    MAAdView* bannerView = [_bannerViews objectForKey:unitId];
    
    if (bannerView == nil) {
        bannerShowPending_ = true;
        [self requestBannerLoad:unitIdView];
        return;
    }
    
    if (_currentBannerView == bannerView) {
        _currentBannerView.hidden = false;
        return;
    }

    if (_currentBannerView != nil) {
        _currentBannerView.hidden = true;
        [_currentBannerView removeFromSuperview];
        _currentBannerView = nil;
    }
        
    [self.currentParentView addSubview:bannerView];
    
//    // Banner height on iPhone and iPad is 50 and 90, respectively
//    CGFloat height = (UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPad) ? 90 : 50;
//    CGRect frame = UIEdgeInsetsInsetRect(self.currentParentView.frame, self.parentView.safeAreaInsets);
//    CGFloat width = CGRectGetWidth(UIScreen.mainScreen.bounds);
//    auto bannerRect = CGRectMake(frame.origin.x, frame.origin.y, width, height);
    
//    bannerRect_.h = height;
    
//    auto bannerFrame2 = toCGRect(bannerRect_);
    auto cgBannerRect = [ManglAdNetwork.instance getBannerRectCG];

    //bannerView.frame = CGRectMake(frame.origin.x, frame.origin.y, width, height);
    
    bannerView.frame = cgBannerRect;
    
    bannerView.hidden = false;
    [bannerView stopAutoRefresh];
    _currentBannerView = bannerView;
}

- (void)requestBannerRemove {
    if (_currentBannerView) {
        _currentBannerView.hidden = true;
        [self.currentBannerView removeFromSuperview];
        self.currentBannerView = nil;
    }
    
}

//
// MARK: Interstitials
//
- (bool)isInterstitialReady:(MStrView)unitId {
#if defined(_DEBUG) && defined(MANGL_ADS_APPLOVIN_MEDIATION_DEBUGGER)
    return true;
#endif
    
    for (id key in _interstitials) {
        if ([_interstitials objectForKey:key] != nil) {
            auto statusIt = interstitialStatuses_.find(toString(key));
            if (statusIt == interstitialStatuses_.end() || !statusIt->second.shown)
                return true;
        }
    }

    return false;
}

- (void)requestInterstitialLoad:(MStrView)aUnitId {
    MDebugFunc("UnitId: %s", aUnitId);
    
    auto unitId = toNSString(aUnitId);

    MAInterstitialAd* interstitial = [self.interstitials objectForKey:unitId];
    
    if (!interstitial) {
        interstitial = [[MAInterstitialAd alloc] initWithAdUnitIdentifier:unitId];
        interstitial.delegate = self;
        
        self.interstitials[unitId] = interstitial;
        
        interstitialStatuses_[String{aUnitId}] = {};
    }
    
    [interstitial loadAd];
}

- (void)requestInterstitialShow:(MStrView)sUnitId {
    MDebugFunc("UnitId: %s", sUnitId);
    
    auto unitId = toNSString(sUnitId);

    MAInterstitialAd* interstitial = [self.interstitials objectForKey:unitId];

    if (!interstitial) {
        MDebugTrap();
        return;
    }

    if ([interstitial isReady]) {
        auto& status = interstitialStatuses_[String{sUnitId}];
        status.shown = true;
        [interstitial showAd];
    }
}


//
// MARK: App Open
//

- (bool)isAppOpenReady:(MStrView)unitId {
    for (id key in _appopens) {
        if ([_appopens objectForKey:key] != nil)
            return YES;
    }

    return NO;
}

- (void)requestAppOpenLoad:(MStrView)sUnitId {
    MDebugFunc("UnitId: %s", sUnitId);
    
    auto nsUnitId = toNSString(sUnitId);

    MAAppOpenAd* appopen = [_appopens objectForKey:nsUnitId];
    
    if (!appopen) {
        appopen = [[MAAppOpenAd alloc] initWithAdUnitIdentifier:nsUnitId];
        appopen.delegate = self;
        
        _appopens[nsUnitId] = appopen;
        
        appopenStatuses_[String{sUnitId}] = {};
    }
    
    [appopen loadAd];
}

- (void)requestAppOpenShow:(MStrView)sUnitId {
    MDebugFunc("UnitId: %s", sUnitId);
    
    auto nsUnitId = toNSString(sUnitId);

    MAAppOpenAd* appopen = [_appopens objectForKey:nsUnitId];

    if (!appopen) {
        MDebugTrap();
        return;
    }

    if ([appopen isReady]) {
        auto& status = appopenStatuses_[String{sUnitId}];
        status.shown = true;
        [appopen showAd];
    }

}

//
// MARK: Rewarded
//
- (bool)isRewardedReady:(MStrView)unitId {
    // TODO: implement
    return false;
}



//
// MARK: MAAdDelegate Protocol
//
- (void)didLoadAd:(nonnull MAAd *)ad {
    MDebugFunc("UnitId: %s", ad.adUnitIdentifier);
    
    if (ad.format == MAAdFormat.banner) {
        if (bannerShowPending_) {
            bannerShowPending_ = false;
            [self requestBannerShow:toStringView(ad.adUnitIdentifier)];
        }
        [ManglAdNetwork.instance onBannerLoaded:self unitId:ad.adUnitIdentifier];
    }
    
    if (ad.format == MAAdFormat.interstitial) {
        auto interstitialIt = interstitialStatuses_.find(toString(ad.adUnitIdentifier));
        if (interstitialIt != interstitialStatuses_.end()) {
            interstitialIt->second.shown = false;
        }
        [ManglAdNetwork.instance onInterstitialLoaded:self unitId:ad.adUnitIdentifier];
    }
    
    if (ad.format == MAAdFormat.appOpen) {
        auto it = appopenStatuses_.find(toString(ad.adUnitIdentifier));
        if (it != appopenStatuses_.end()) {
            it->second.shown = false;
        }
        [ManglAdNetwork.instance onAppOpenLoaded:self unitId:ad.adUnitIdentifier];
    }

}

- (void)didFailToLoadAdForAdUnitIdentifier:(nonnull NSString *)unitId withError:(nonnull MAError *)error {
    MDebugFunc("UnitId: %s, Error: %s", unitId, error.description);
    
    NSDictionary *userInfo = @{
        NSLocalizedDescriptionKey: error.message,
    };

    
    NSError* nsError = [NSError errorWithDomain:@"com.mangl.ads" code:error.code userInfo:userInfo];
    
    id interstitial = [_interstitials objectForKey:unitId];
    if (interstitial != nil) {
        [ManglAdNetwork.instance onInterstitialLoadError:self unitId:unitId withError:nsError];
        return;
    }
    
    id appopen = [_appopens objectForKey:unitId];
    if (appopen != nil) {
        [ManglAdNetwork.instance onAppOpenLoadError:self unitId:unitId withError:nsError];
        return;
    }

    
    id banner = [_bannerViews objectForKey:unitId];
    if (banner != nil) {
        [ManglAdNetwork.instance onBannerLoadError:self unitId:unitId withError:nsError];
    }
}


- (void)didClickAd:(nonnull MAAd *)ad {
    MDebugFunc("UnitId: %s", ad.adUnitIdentifier);
    
    if (ad.format == MAAdFormat.banner) {
        [ManglAdNetwork.instance onBannerClicked:self unitId:ad.adUnitIdentifier];
        [ManglAdNetwork.instance onBannerWillShowFullScreen:self unitId:ad.adUnitIdentifier];
    }
}

- (void)didFailToDisplayAd:(nonnull MAAd *)ad withError:(nonnull MAError *)error {
    MDebugFunc("UnitId: %s, Error: %s", ad.adUnitIdentifier, error.description);
}



- (void)didDisplayAd:(nonnull MAAd *)ad {
    MDebugFunc("UnitId: %s", ad.adUnitIdentifier);
    
    if (ad.format == MAAdFormat.banner) {
        //[super notifyBannerWillShowFullScreen];
    }
    
    if (ad.format == MAAdFormat.interstitial) {
        auto interstitialIt = interstitialStatuses_.find(toString(ad.adUnitIdentifier));
        if (interstitialIt != interstitialStatuses_.end()) {
            interstitialIt->second.shown = true;
        }
        
        [self recordFullScreenAdDisplay:ad.adUnitIdentifier type:ManglAdType::Interstitial];
        [ManglAdNetwork.instance onInterstitialWillPresentFullScreen:self unitId:ad.adUnitIdentifier];
    }
    
    if (ad.format == MAAdFormat.appOpen) {
        auto it = appopenStatuses_.find(toString(ad.adUnitIdentifier));
        if (it != appopenStatuses_.end()) {
            it->second.shown = true;
        }
        
        [self recordFullScreenAdDisplay:ad.adUnitIdentifier type:ManglAdType::AppOpen];
        [ManglAdNetwork.instance onAppOpenWillPresentFullScreen:self unitId:ad.adUnitIdentifier];
    }

}



- (void)didHideAd:(nonnull MAAd *)ad {
    MDebugFunc("UnitId: %s", ad.adUnitIdentifier);

    if (ad.format == MAAdFormat.interstitial) {
        [ManglAdNetwork.instance onInterstitialDidDismissFullScreen:self unitId:ad.adUnitIdentifier];
    }
    
    if (ad.format == MAAdFormat.appOpen) {
        [ManglAdNetwork.instance onAppOpenDidDismissFullScreen:self unitId:ad.adUnitIdentifier];
    }
}



// MARK: MAAdViewAdDelegate Protocol
- (void)didCollapseAd:(nonnull MAAd *)ad {
    MDebugFunc("UnitId: %s", ad.adUnitIdentifier);
    
    if (ad.format == MAAdFormat.interstitial) {
        [ManglAdNetwork.instance onInterstitialDidDismissFullScreen:self unitId:ad.adUnitIdentifier];
    }
    
    if (ad.format == MAAdFormat.appOpen) {
        [ManglAdNetwork.instance onAppOpenDidDismissFullScreen:self unitId:ad.adUnitIdentifier];
    }
}

- (void)didExpandAd:(nonnull MAAd *)ad {
    MDebugFunc("UnitId: %s", ad.adUnitIdentifier);

    
    if (ad.format == MAAdFormat.banner) {
        [ManglAdNetwork.instance onBannerWillShowFullScreen:self unitId:ad.adUnitIdentifier];
    }
    
    if (ad.format == MAAdFormat.interstitial) {
        auto it = interstitialStatuses_.find(toString(ad.adUnitIdentifier));
        if (it != interstitialStatuses_.end()) {
            it->second.shown = true;
        }
        [self recordFullScreenAdDisplay:ad.adUnitIdentifier type:ManglAdType::Interstitial];
        [ManglAdNetwork.instance onInterstitialWillPresentFullScreen:self unitId:ad.adUnitIdentifier];
    }
    
    if (ad.format == MAAdFormat.appOpen) {
        auto it = appopenStatuses_.find(toString(ad.adUnitIdentifier));
        if (it != appopenStatuses_.end()) {
            it->second.shown = true;
        }
        [self recordFullScreenAdDisplay:ad.adUnitIdentifier type:ManglAdType::AppOpen];
        [ManglAdNetwork.instance onAppOpenWillPresentFullScreen:self unitId:ad.adUnitIdentifier];
    }

}


@end
#endif


