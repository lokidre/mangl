#import "mangl_main_app.h"

#import "mangl_main_window.h"

#include "../core/mangl_setup.h"
#include "../core/mangl_util.h"

#include "../core/mangl_framework.h"
#include "../core/main/mangl_main.h"
#include "../core/mangl_ads.h"

#include "mangl_hooks_ios.h"

#ifdef MANGL_APPLE_PAY
@import PassKit;
#endif


#ifdef MANGL_FIREBASE
@import FirebaseCore;
@import FirebaseCrashlytics;
@import FirebaseAnalytics;
#endif


namespace mangl::mangl_internal {
bool advertisingIdRequested_{};
}  // namespace

using namespace mangl;

@interface ManglMainApplicationDelegate ()

@property(nonatomic, getter=window) ManglMainWindow* mainWindow;
@property (nonatomic) NSDictionary* launchOptions;

@end


@implementation ManglMainApplicationDelegate

@synthesize mainWindow;


- (void)ctor {
    mangl_main();
    Framework::instance()->init();
}

// MARK: Launch Continue
- (void)applicationLaunchContinue {

#if MANGL_FIREBASE
    [FIRApp configure];
    
    [FIRAnalytics setConsent:@{
        FIRConsentTypeAnalyticsStorage : FIRConsentStatusGranted,
        FIRConsentTypeAdStorage : FIRConsentStatusGranted,
        FIRConsentTypeAdUserData : FIRConsentStatusGranted,
        FIRConsentTypeAdPersonalization : FIRConsentStatusGranted,
    }];
    
#ifdef _DEBUG
    [FIRCrashlytics.crashlytics setCrashlyticsCollectionEnabled:NO];
#endif
    
//    int delaySec = 10;
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delaySec * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
//        @[][1];
//    });

#endif

#ifdef MANGL_APPLE_PAY
    // Check apple pay
    auto canMakePayments = PKPaymentAuthorizationViewController.canMakePayments;
    MDebugPrint("Apple Pay, Can Make Payments: %d", canMakePayments);
#endif

    auto hooks = ManglIosHooks.instance;
    if (hooks.hooks != nil) {
        for (id<ManglIosMainHook> hook in hooks.hooks) {
            if ([hook respondsToSelector:@selector(didFinishLaunchingWithOptions:)]) {
                [hook didFinishLaunchingWithOptions:_launchOptions];
            }
        }
    }
}


- (void)applicationLaunchContinueTimer:(NSTimer *)timer {
    [self applicationLaunchContinue];
}


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    MDebugFunc();
    
    _launchOptions = launchOptions;
  
    [self ctor];

    self.mainWindow = [[ManglMainWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.mainWindow.backgroundColor = UIColor.blackColor;
    [self.mainWindow makeKeyAndVisible];

//#if 1   // Using timer to continue initalization
//    // Continue with the initialization
//    auto userInfo = @{
//        @"appliction": application,
//        @"launchOptions": launchOptions ? launchOptions : NSNull.null,
//    };
//
//    [NSTimer scheduledTimerWithTimeInterval:0
//                                     target:self
//                                   selector:@selector(applicationLaunchContinueTimer:)
//                                   userInfo:userInfo
//                                    repeats:false];
//#else
    [self applicationLaunchContinue];
//#endif

    return true;
}


- (void)applicationWillTerminate {
    MDebugFunc();
    
    auto hooks = ManglIosHooks.instance;
    if (hooks.hooks != nil) {
        for (id<ManglIosMainHook> hook in hooks.hooks) {
            if ([hook respondsToSelector:@selector(applicationWillTerminate)]) {
                [hook applicationWillTerminate];
            }
        }
    }

    Framework::instance()->onTerminate();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    MDebugFunc();
    Framework::instance()->onTerminate();
}

- (void)applicationWillResignActive:(UIApplication *)application {
    MDebugFunc();
    [ManglMainViewController.instance onApplicationWillResignActive];
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
    MDebugFunc();
    [ManglMainViewController.instance onApplicationDidBecomeActive];
    
    auto hooks = ManglIosHooks.instance;
    if (hooks.hooks != nil) {
        for (id<ManglIosMainHook> hook in hooks.hooks) {
            if ([hook respondsToSelector:@selector(applicationDidBecomeActive)]) {
                [hook applicationDidBecomeActive];
            }
        }
    }

}


@end
