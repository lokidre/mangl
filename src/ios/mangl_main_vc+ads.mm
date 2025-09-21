#import "mangl_main_vc+ads.h"

#include "../core/mangl_ads.h"


#if MANGL_ADS

@import AppTrackingTransparency;
@import AdSupport;

@import UserMessagingPlatform;


#import "mangl_main_vc.h"
#import "ads/mangl_ad_network.h"

using namespace mangl;

@implementation ManglMainViewController(Ads)

- (void)loadAdsSdk {
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        [ManglAdNetwork.instance onInitNetwork];
        
#if _DEBUG
        if ([ASIdentifierManager class]) {
            NSUUID *adId = ASIdentifierManager.sharedManager.advertisingIdentifier;
            MDebugPrint("AdvertisingIdentifier (IDFA): %s", adId.UUIDString);
        }
#endif

    });
}

- (void)viewDidLoadAds {
}

- (void)viewDidAppearAds {
//    Ads::bannerLayoutChangedCallback = ^(){
//        [ManglAdNetwork.instance moveBanner: Ads::bannerLayout()];
//    };
    
    if (Ads::isPermanentDisabled())
        return;
    
    //
    // Consent management
    //
    // Create a UMPRequestParameters object.
    UMPRequestParameters *parameters = [[UMPRequestParameters alloc] init];
    // Set tag for under age of consent. NO means users are not under age of consent.
    parameters.tagForUnderAgeOfConsent = NO;
    
#if _DEBUG
//    UMPDebugSettings *debugSettings = [[UMPDebugSettings alloc] init];
//    //debugSettings.testDeviceIdentifiers = @[ @"0CE82990-7035-405E-817A-024357C8DD78" ];
//    debugSettings.geography = UMPDebugGeographyEEA;
//    parameters.debugSettings = debugSettings;
    
    //[UMPConsentInformation.sharedInstance reset];
#endif
    
    // Request an update for the consent information.
    __weak __typeof__(self) weakSelf = self;
    
    [UMPConsentInformation.sharedInstance requestConsentInfoUpdateWithParameters:parameters
                                                               completionHandler:^(NSError *_Nullable requestConsentError) {
        __strong __typeof__(self) strongSelf = weakSelf;
        if (!strongSelf) {
            return;
        }
        
        if (requestConsentError) {
            // Consent gathering failed.
            NSLog(@"Error: %@", requestConsentError.localizedDescription);
            
            if ([ATTrackingManager class]) {
                [ATTrackingManager requestTrackingAuthorizationWithCompletionHandler:^(ATTrackingManagerAuthorizationStatus status) {
                    [strongSelf loadAdsSdk];
                }];
            } else {
                [strongSelf loadAdsSdk];
            }
            
            return;
        }
        
        [UMPConsentForm loadAndPresentIfRequiredFromViewController:strongSelf
                                                 completionHandler:^(NSError *loadAndPresentError) {
            if (loadAndPresentError) {
                // Consent gathering failed.
                NSLog(@"Error: %@", loadAndPresentError.localizedDescription);
                [strongSelf loadAdsSdk];
                return;
            }

            // Consent has been gathered.
            __strong __typeof__(self) strongSelf = weakSelf;
            if (!strongSelf) {
                return;
            }

            if (UMPConsentInformation.sharedInstance.canRequestAds) {
                [strongSelf loadAdsSdk];
            }
        }];
    }];
    
    if (UMPConsentInformation.sharedInstance.canRequestAds) {
        [self loadAdsSdk];
    }
}

- (void)onApplicationDidBecomeActiveAds {
    [ManglAdNetwork.instance onApplicationDidBecomeActive];
}

- (void)onApplicationWillResignActiveAds {
    [ManglAdNetwork.instance onApplicationWillResignActive];
}

@end

#endif

