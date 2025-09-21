#import "mangl_ad_adaptor_admob.h"
#import "../mangl_ad_network.h"

#if MANGL_ADS_ADMOB || MANGL_ADS_MEDIATION_ADMOB

using namespace mangl;


@interface ManglAdAdaptorAdMob() {
    bool bannerShowPending_;
}

@property (nonatomic, strong) NSMutableDictionary<NSString*, GADBannerView*> *bannerViews;
@property (nonatomic, strong) NSMutableDictionary<NSString*, GADRequest*> *bannerRequests;
@property (nonatomic, weak) GADBannerView* currentBannerView;

@property (nonatomic, strong) NSMutableDictionary<NSString*, GADInterstitialAd*> *interstitials;
@property (nonatomic, strong) NSMutableDictionary<NSString*, GADRequest*> *interstitialRequests;

@property (nonatomic, strong) NSMutableDictionary<NSString*, GADAppOpenAd*> *appOpens;
//@property (nonatomic, strong, nullable) GADAppOpenAd *appOpenAd;

@property (nonatomic, strong) NSMutableDictionary<NSString*, GADRewardedAd*> *rewarded;
@property (nonatomic, strong) NSMutableDictionary<NSString*, GADRequest*> *rewardedRequests;


@end


@implementation ManglAdAdaptorAdMob

- (id)init {
    self = [super init];
    
    self->name = ManglAdNetworkNameAdmob;
    
    _currentBannerView = nil;
    
    _bannerViews = [[NSMutableDictionary<NSString*, GADBannerView*> alloc] init];
    _bannerRequests = [[NSMutableDictionary alloc] init];

    _interstitials = [[NSMutableDictionary alloc] init];
    _interstitialRequests = [[NSMutableDictionary alloc] init];

    _appOpens = [[NSMutableDictionary alloc] init];

    _rewarded = [[NSMutableDictionary alloc] init];
    _rewardedRequests = [[NSMutableDictionary alloc] init];
    
    bannerShowPending_ = false;
    
    return self;
}

- (void)requestNetworkReset {
    if (_currentBannerView) {
        _currentBannerView.hidden = true;
        [_currentBannerView removeFromSuperview];
        _currentBannerView = nil;
    }
    
    [_bannerViews removeAllObjects];
    [_bannerRequests removeAllObjects];
    
    [_interstitials removeAllObjects];
    [_interstitialRequests removeAllObjects];

    [_appOpens removeAllObjects];

    [_rewarded removeAllObjects];
    [_rewardedRequests removeAllObjects];
}



- (void)onInitNetwork {
//    GADMobileAds.sharedInstance.applicationVolume = 0.001;
    
    [GADMobileAds.sharedInstance startWithCompletionHandler:^(GADInitializationStatus *_Nonnull status) {
        MDebugFunc();
        
        NSDictionary *adapterStatuses = [status adapterStatusesByClassName];

        for (NSString *adapter in adapterStatuses) {
            GADAdapterStatus *adapterStatus = adapterStatuses[adapter];
            NSLog(@"Adapter Name: %@, Description: %@, Latency: %f", adapter,
                  adapterStatus.description, adapterStatus.latency);
        }
        
        GADMobileAds.sharedInstance.applicationVolume = 0.001;
//        GADMobileAds.sharedInstance.applicationMuted = true;

        [super processAdaptorInitCompletion];
    }];
}

//- (void)setBannerLayout:(mangl::RectA)rect parentView:(UIView *)parentView {
//    bannerRect_ = rect;
////    _parentView = parentView;
//}





// MARK: Banners

- (void)requestBannerLoad:(MStrView)unitIdView {
    auto unitId = toNSString(unitIdView);

    GADBannerView* bannerView = [_bannerViews objectForKey:unitId];
    GADRequest* request = [_bannerRequests objectForKey:unitId];

    if (!bannerView) {
        auto cgBannerRect = [ManglAdNetwork.instance getBannerRectCG];
        
        bannerView = [[GADBannerView alloc] initWithAdSize:GADAdSizeFromCGSize(cgBannerRect.size)
                                                    origin:cgBannerRect.origin];
        
        
        // From Google Docs
        CGRect frame = UIEdgeInsetsInsetRect(self.currentParentView.frame, self.currentParentView.safeAreaInsets);
        CGFloat viewWidth = frame.size.width;
        GADAdSize adaptiveSize = GADCurrentOrientationAnchoredAdaptiveBannerAdSizeWithWidth(viewWidth);
//        bannerView = [[GADBannerView alloc] initWithAdSize:adaptiveSize];
        bannerView = [[GADBannerView alloc] initWithAdSize:adaptiveSize
                                                    origin:cgBannerRect.origin];

        
        bannerView.adUnitID = unitId;
        bannerView.delegate = self;
        bannerView.autoloadEnabled = false;
        bannerView.rootViewController = self.rootViewController;
        
        _bannerViews[unitId] = bannerView;
        
        request = [[GADRequest alloc] init];

        _bannerRequests[unitId] = request;
    }
    
    [bannerView loadRequest:request];
}

- (void)requestBannerShow:(mangl::StrView)unitId {
    auto nsUnitId = toNSString(unitId);

    GADBannerView* bannerView = [_bannerViews objectForKey:nsUnitId];
    
    // If banner is not loaded, load it
    if (bannerView == nil) {
        bannerShowPending_ = true;
        [self requestBannerLoad:unitId];
        return;
    }
    
    // If error happened, reload the banner
    if (bannerView.responseInfo == nil) {
        bannerShowPending_ = true;
        [self requestBannerLoad:unitId];
        return;
    }
    
    if (_currentBannerView == bannerView)
        return;

    if (_currentBannerView != nil) {
        _currentBannerView.hidden = true;
        [_currentBannerView removeFromSuperview];
        _currentBannerView = nil;
    }
  
    
    [self.currentParentView addSubview:bannerView];

//    // From Google Docs
//    bannerView.translatesAutoresizingMaskIntoConstraints = NO;
//    [self.parentView addConstraints:@[
//        [NSLayoutConstraint constraintWithItem:bannerView
//                                   attribute:NSLayoutAttributeBottom
//                                   relatedBy:NSLayoutRelationEqual
//                                      toItem:self.parentView.safeAreaLayoutGuide
//                                   attribute:NSLayoutAttributeBottom
//                                  multiplier:1
//                                    constant:0],
//        [NSLayoutConstraint constraintWithItem:bannerView
//                                   attribute:NSLayoutAttributeCenterX
//                                   relatedBy:NSLayoutRelationEqual
//                                      toItem:self.parentView
//                                   attribute:NSLayoutAttributeCenterX
//                                  multiplier:1
//                                    constant:0]
//                                    ]];
    
    
    //[self.currentParentView addSubview:bannerView];
    bannerView.hidden = false;
    _currentBannerView = bannerView;
}

- (void)requestBannerRemove {
    if (_currentBannerView) {
        _currentBannerView.hidden = true;
        [_currentBannerView removeFromSuperview];
        _currentBannerView = nil;
    }
}



// MARK: Interstitials
- (bool)isInterstitialReady:(MStrView)unitId {
    for (id key in _interstitials) {
        if ([_interstitials objectForKey:key] != nil)
            return YES;
    }

    return NO;
}

- (void)requestInterstitialLoad:(MStrView)unitId {
    MDebugFunc("UnitId: %s", unitId);
    
    auto nsUnitId = toNSString(unitId);

    GADRequest *request = [GADRequest request];

    _interstitials[nsUnitId] = nil;
    
    [GADInterstitialAd loadWithAdUnitID:nsUnitId
                                request:request
                      completionHandler:^(GADInterstitialAd *ad, NSError *error) {
        if (error) {
            MDebugPrint("Interstitial load failed: %s, %s", nsUnitId, error.localizedDescription);
            [ManglAdNetwork.instance onInterstitialLoadError:self unitId:nsUnitId withError:error];
            [self recordFullScreenAdError:nsUnitId type:ManglAdType::Interstitial];
            return;
        }
        
        MDebugPrint("Interstitial loaded: %s", nsUnitId);
        
        [self recordFullScreenAdLoad:nsUnitId type:ManglAdType::Interstitial];

        self.interstitials[nsUnitId] = ad;
        self.interstitials[nsUnitId].fullScreenContentDelegate = self;
        [ManglAdNetwork.instance onInterstitialLoaded:self unitId:nsUnitId];
    }];
}

- (void)requestInterstitialShow:(MStrView)unitId {
    auto nsUnitId = toNSString(unitId);
    GADInterstitialAd* interstitial = [_interstitials objectForKey:nsUnitId];
    
    if (!interstitial) {
//        MDebugTrap();
        return;
    }
    
    auto canPresent = [interstitial canPresentFromRootViewController:self.rootViewController error:nil];
    
    if (!canPresent) {
        [self requestInterstitialLoad:unitId];
        return;
    }
    
    [interstitial presentFromRootViewController:self.rootViewController];
}

//
// MARK: App Open
//
// App Open
- (bool)isAppOpenReady:(MStrView)unitId {
    for (id key in _appOpens) {
        if ([_appOpens objectForKey:key] != nil)
            return YES;
    }

    return NO;
}

- (void)requestAppOpenLoad:(MStrView)unitId {
    MDebugFunc("UnitId: %s", unitId);
    
    auto nsUnitId = toNSString(unitId);
    
    [GADAppOpenAd loadWithAdUnitID:nsUnitId
                           request:[GADRequest request]
                 completionHandler:^(GADAppOpenAd * _Nullable ad, NSError * _Nullable error) {
//      self.isLoadingAd = NO;
        if (error) {
            MDebugPrint("AppOpen load failed: %s, %s", nsUnitId, error.localizedDescription);
            [ManglAdNetwork.instance onAppOpenLoadError:self unitId:nsUnitId withError:error];
            [self recordFullScreenAdError:nsUnitId type:ManglAdType::AppOpen];
            return;
        }

        MDebugPrint("Interstitial loaded: %s", nsUnitId);
        
        [self recordFullScreenAdLoad:nsUnitId type:ManglAdType::Interstitial];

        self.appOpens[nsUnitId] = ad;
        self.appOpens[nsUnitId].fullScreenContentDelegate = self;
        [ManglAdNetwork.instance onAppOpenLoaded:self unitId:nsUnitId];
    }];

}

- (void)requestAppOpenShow:(MStrView)unitId {
    auto nsUnitId = toNSString(unitId);
    GADAppOpenAd* appOpen = [_appOpens objectForKey:nsUnitId];
    
    if (!appOpen) {
//        MDebugTrap();
        return;
    }
    
    [appOpen presentFromRootViewController:nil];
}

//- (void)requestAppOpenDismiss:(MStrView)unitId {
//    auto nsUnitId = toNSString(unitId);
//    GADAppOpenAd* appOpen = [_appOpens objectForKey:nsUnitId];
//    
//    if (!appOpen) {
////        MDebugTrap();
//        return;
//    }
//    
//    [appOpen presentFromRootViewController:nil];
//}


//
// MARK: Rewarded
//
- (bool)isRewardedReady:(MStrView)unitId {
    auto nsUnitId = toNSString(unitId);
    
    GADRewardedAd* rewarded = [_rewarded objectForKey:nsUnitId];
        
    auto canShow = [rewarded canPresentFromRootViewController:self.rootViewController error:nil];
        
    if (canShow)
        return true;

    return false;
}

- (void)requestRewardedLoad:(MStrView)aUnitId {
    auto nsUnitId = toNSString(aUnitId);

    GADRequest *request = [GADRequest request];

    self.rewarded[nsUnitId] = nil;
    
    [GADRewardedAd loadWithAdUnitID:nsUnitId
                            request:request
                  completionHandler:^(GADRewardedAd *ad, NSError *error) {
        if (error) {
            MDebugPrint("Rewarded load failed: %s, %s", nsUnitId, error.localizedDescription);
            [ManglAdNetwork.instance onRewardedLoadError:self unitId:nsUnitId withError:error];
            return;
        }
        
        MDebugPrint("Rewarded loaded: %s", nsUnitId);
        
        [self recordFullScreenAdLoad:nsUnitId  type:ManglAdType::Rewarded];
        
        self.rewarded[nsUnitId] = ad;
        self.rewarded[nsUnitId].fullScreenContentDelegate = self;
        [ManglAdNetwork.instance onRewardedLoaded:self unitId:nsUnitId];
    }];
}

- (void)requestRewardedShow:(mangl::StrView)sUnitId {
    auto unitId = toNSString(sUnitId);
    GADRewardedAd* rewarded = [self.rewarded objectForKey:unitId];
    
    if (!rewarded) {
        MDebugTrap();
        return;
    }
    
    [rewarded presentFromRootViewController:self.rootViewController userDidEarnRewardHandler:^{
        GADAdReward *reward = rewarded.adReward;
        [ManglAdNetwork.instance onRewardEarned:self
                                         unitId:unitId
                                         reward:reward.type
                                         amount:reward.amount.doubleValue];
    }];
}



//
// MARK: GADBannerViewDelegate
//
- (void)bannerViewDidReceiveAd:(nonnull GADBannerView *)bannerView {
    MDebugFunc("Unit: %s", bannerView.adUnitID);
    if (bannerShowPending_) {
        bannerShowPending_ = false;
        [self requestBannerShow:toStringView(bannerView.adUnitID)];
    }
    [ManglAdNetwork.instance onBannerLoaded:self unitId:bannerView.adUnitID];
}

- (void)bannerView:(nonnull GADBannerView *)bannerView didFailToReceiveAdWithError:(nonnull NSError *)error {
    MDebugFunc("Unit: %s, Error: %s", bannerView.adUnitID, error.localizedDescription);
    [ManglAdNetwork.instance onBannerLoadError:self unitId:bannerView.adUnitID withError:error];
}

// Tells the delegate that an impression has been recorded for an ad.
- (void)bannerViewDidRecordImpression:(nonnull GADBannerView *)bannerView {
    MDebugFunc();
}

// Tells the delegate that a click has been recorded for the ad.
- (void)bannerViewDidRecordClick:(nonnull GADBannerView *)bannerView {
    MDebugFunc();
    [ManglAdNetwork.instance onBannerClicked:self unitId:bannerView.adUnitID];
}

- (void)bannerViewWillPresentScreen:(GADBannerView *)bannerView {
    MDebugFunc();
    [ManglAdNetwork.instance onBannerWillShowFullScreen:self unitId:bannerView.adUnitID];
}

- (void)bannerViewWillDismissScreen:(GADBannerView *)bannerView {
    MDebugFunc();
}

- (void)bannerViewDidDismissScreen:(GADBannerView *)bannerView {
    MDebugFunc();
    [ManglAdNetwork.instance onBannerDidDismissFullScreen:self unitId:bannerView.adUnitID];
}




// MARK: GADFullScreenContentDelegate
// Tells the delegate that an impression has been recorded for the ad.
- (void)adDidRecordImpression:(nonnull id<GADFullScreenPresentingAd>)ad {
    if ([ad isKindOfClass:GADInterstitialAd.class]) {
        auto interstitial = (GADInterstitialAd *)ad;
        MDebugFunc("UnitId: %s", interstitial.adUnitID);
    }
}

// Tells the delegate that a click has been recorded for the ad.
- (void)adDidRecordClick:(nonnull id<GADFullScreenPresentingAd>)ad {
    if ([ad isKindOfClass:GADInterstitialAd.class]) {
        auto interstitial = (GADInterstitialAd *)ad;
        MDebugFunc("UnitId: %s", interstitial.adUnitID);
    }
}

// Tells the delegate that the ad failed to present full screen content.
- (void)ad:(nonnull id<GADFullScreenPresentingAd>)ad didFailToPresentFullScreenContentWithError:(nonnull NSError *)error {
    MDebugFunc();
    
    if ([ad isKindOfClass:GADInterstitialAd.class]) {
        auto interstitial = (GADInterstitialAd *)ad;
        MDebugFunc("UnitId: %s", interstitial.adUnitID);
        self.interstitials[interstitial.adUnitID] = nil;
        [self recordFullScreenAdError:interstitial.adUnitID type:ManglAdType::Interstitial];
        [ManglAdNetwork.instance onInterstitialShowError:self unitId:interstitial.adUnitID withError:error];
    }
    
    if ([ad isKindOfClass:GADAppOpenAd.class]) {
        auto appOpen = (GADAppOpenAd *)ad;
        MDebugFunc("UnitId: %s", appOpen.adUnitID);
        self.appOpens[appOpen.adUnitID] = nil;
        [self recordFullScreenAdError:appOpen.adUnitID type:ManglAdType::AppOpen];
        [ManglAdNetwork.instance onAppOpenShowError:self unitId:appOpen.adUnitID withError:error];
    }

    
    if ([ad isKindOfClass:GADRewardedAd.class]) {
        auto rewarded = (GADRewardedAd *)ad;
        MDebugFunc("UnitId: %s", rewarded.adUnitID);
        self.rewarded[rewarded.adUnitID] = nil;
        [self recordFullScreenAdError:rewarded.adUnitID type:ManglAdType::Rewarded];
        [ManglAdNetwork.instance onRewardedShowError:self unitId:rewarded.adUnitID withError:error];
    }

}

// Tells the delegate that the ad will present full screen content.
- (void)adWillPresentFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
//    MDebugFunc();

//    if ([ad isKindOfClass:GADBannerView.class]) {
//        auto banner = (GADBannerView *)ad;
//        MDebugFunc("UnitId: %s", banner.adUnitID);
//        [super notifyBannerWillShowFullScreen];
//    }
    
    if ([ad isKindOfClass:GADInterstitialAd.class]) {
        auto cad = (GADInterstitialAd *)ad;
        MDebugFunc("UnitId: %s", cad.adUnitID);
        [self recordFullScreenAdDisplay:cad.adUnitID type:ManglAdType::Interstitial];
        [ManglAdNetwork.instance onInterstitialWillPresentFullScreen:self unitId:cad.adUnitID];
    }
    
    if ([ad isKindOfClass:GADAppOpenAd.class]) {
        auto cad = (GADAppOpenAd *)ad;
        MDebugFunc("UnitId: %s", cad.adUnitID);
        [self recordFullScreenAdDisplay:cad.adUnitID type:ManglAdType::AppOpen];
        [ManglAdNetwork.instance onAppOpenWillPresentFullScreen:self unitId:cad.adUnitID];
    }
    
    if ([ad isKindOfClass:GADRewardedAd.class]) {
        auto cad = (GADRewardedAd *)ad;
        MDebugFunc("UnitId: %s", cad.adUnitID);
        [self recordFullScreenAdDisplay:cad.adUnitID type:ManglAdType::Rewarded];
        [ManglAdNetwork.instance onRewardedWillPresentFullScreen:self unitId:cad.adUnitID];
    }
}


// Tells the delegate that the ad dismissed full screen content.
- (void)adDidDismissFullScreenContent:(nonnull id<GADFullScreenPresentingAd>)ad {
//    if ([ad isKindOfClass:GADBannerView.class]) {
//        auto banner = (GADBannerView *)ad;
//        MDebugFunc("UnitId: %s", banner.adUnitID);
//        [super notifyBannerDidDismissFullScreen];
//    }
    
    if ([ad isKindOfClass:GADAppOpenAd.class]) {
        auto cad = (GADAppOpenAd *)ad;
        self.appOpens[cad.adUnitID] = nil;
        MDebugFunc("UnitId: %s", cad.adUnitID);
        [ManglAdNetwork.instance onAppOpenDidDismissFullScreen:self unitId:cad.adUnitID];
    }


    if ([ad isKindOfClass:GADInterstitialAd.class]) {
        auto cad = (GADInterstitialAd *)ad;
        self.interstitials[cad.adUnitID] = nil;
        MDebugFunc("UnitId: %s", cad.adUnitID);
        [ManglAdNetwork.instance onInterstitialDidDismissFullScreen:self unitId:cad.adUnitID];
    }
}

@end

#endif

