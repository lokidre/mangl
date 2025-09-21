#include "mangl_rate_annoyer.h"

#if MANGL_RATE_ANNOYER

#if APEAL_OS_IPHONE
#import "../ios/mangl_main_vc.h"
#else
@import Cocoa;
#endif

@import StoreKit;

#include "mangl_reachability.h"

//#include <apeal/str.hpp>

#define key_rated         "key_rate_rated"           // rated flag (bool)
#define key_launch_date   "key_rate_launch_date"     // first launch date (double)
#define key_launch_count  "key_rate_launch_count"    // launch count (int)
#define key_track_version "key_rate_track_version"   // the version which is being tracked (string)
#define key_declined      "key_rate_declined"        // user declined to rate (bool)

@interface ManglRateAnnoyer ()
@property(nonatomic) NSString *ratingURL;
@end


@implementation ManglRateAnnoyer

+ (ManglRateAnnoyer *)instance
{
    static ManglRateAnnoyer *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ManglRateAnnoyer alloc] init];
        [sharedInstance ctor];
    });
    return sharedInstance;
}

- (void)ctor
{
#if defined(MANGL_APP_ID)
    using namespace apl;

    debug = false;

#if MANGL_RATE_ANNOYER_DEBUG
    debug = true;
#endif

    trackVersion = false;

    _appId = @MANGL_APP_ID;
    _appName = @MANGL_APP_TITLE;   // [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleNameKey] ;
    _promptTitle = [NSString stringWithFormat:@"Rate %@", _appName, nil];

    //#ifdef MANGL_RATE_ANNOYER_TITLE
    //	promptTitle = @MANGL_RATE_ANNOYER_TITLE ;
    //#endif

    _promptMessage =
        [NSString stringWithFormat:
                      @"If you enjoy %@, would you please take a moment to rate it? It won't take much time.\n\nThanks for your support!",
                      _appName,
                      nil];

    _rateButtonTitle = [NSString stringWithFormat:@"Rate %@", _appName, nil];

    _laterButtonTitle = @"Later";
    _declineButtonTitle = @"No, Thanks";

    daysToRate = 4;
    launchesToRate = 32;

#endif   // rate annoyer defined
}


- (void)onUserDeclined
{
    auto defs = NSUserDefaults.standardUserDefaults;
    [defs setBool:true forKey:@key_declined];
    //[defs synchronize];
}

- (void)onUserLater
{
    auto defs = NSUserDefaults.standardUserDefaults;
    // give the user some more time
    [defs setDouble:[[NSDate date] timeIntervalSince1970] forKey:@key_launch_date];
    [defs setInteger:launchesToRate / 2 forKey:@key_launch_count];
    //[defs synchronize];
}

- (void)onUserRate
{
    [self rate_internal];
}

#if APEAL_OS_MAC && !APEAL_OS_IPHONE
- (void)openAppPageWhenAppStoreLaunched
{
    // check if app store is running
    for (NSRunningApplication *app in NSWorkspace.sharedWorkspace.runningApplications) {
        if ([app.bundleIdentifier isEqualToString:@"com.apple.appstore"]) {
            // open app page
            [NSWorkspace.sharedWorkspace performSelector:@selector(openURL:) withObject:self.ratingURL afterDelay:1.0];
            return;
        }
    }

    // try again
    [self performSelector:@selector(openAppPageWhenAppStoreLaunched) withObject:nil afterDelay:0.0];
}
#endif


- (void)showPrompt:(id)sender
{
#if APEAL_OS_IPHONE
    if (@available(iOS 10.3, *)) {
        [SKStoreReviewController requestReview];
        [self onUserLater];
    } else {
        auto alert = [UIAlertController alertControllerWithTitle: _promptTitle
                                                         message: _promptMessage
                                                  preferredStyle: UIAlertControllerStyleAlert];

        auto action = [UIAlertAction actionWithTitle:_declineButtonTitle
                                               style:UIAlertActionStyleDefault
                                             handler:^(UIAlertAction *action) { [self onUserDeclined]; }];
        [alert addAction:action];

        action = [UIAlertAction actionWithTitle: _laterButtonTitle
                                          style: UIAlertActionStyleDefault
                                        handler: ^(UIAlertAction *action) { [self onUserLater]; }];
        [alert addAction:action];

        action = [UIAlertAction actionWithTitle:_rateButtonTitle
                                          style:UIAlertActionStyleDefault
                                        handler:^(UIAlertAction *action) { [self onUserRate]; }];
        [alert addAction:action];

        [ManglMainViewController.instance presentViewController:alert animated:YES completion:nil];
    }
#else
    auto alert = [[NSAlert alloc] init];
    [alert setShowsHelp:NO];
    [alert setAlertStyle:NSAlertStyleInformational];
    //[alert setDelegate:self] ;
    [alert setMessageText:_promptTitle];
    [alert setInformativeText:_promptMessage];
    [alert addButtonWithTitle:_rateButtonTitle];
    [alert addButtonWithTitle:_laterButtonTitle];
    [alert addButtonWithTitle:_declineButtonTitle];
    auto response = [alert runModal];
    switch (response) {
    case NSAlertFirstButtonReturn: [self onUserRate]; break;
    case NSAlertSecondButtonReturn: [self onUserLater]; break;
    case NSAlertThirdButtonReturn: [self onUserDeclined]; break;
    }
#endif
}

+ (void)prompt
{
    [ManglRateAnnoyer.instance showPrompt:0];
}

- (bool)acted_internal
{
    if ([_appId length] == 0)
        @throw([NSException exceptionWithName:@"Rate Annoyer" reason:@"app_id is not set" userInfo:nil]);


    auto defs = NSUserDefaults.standardUserDefaults;
    bool rated = false, declined = false;

    if ([defs objectForKey:@key_rated] != nil)
        rated = [defs boolForKey:@key_rated];

    if ([defs objectForKey:@key_declined] != nil)
        declined = [defs boolForKey:@key_declined];



    // if we are tracking the version, and if the tracking version is not the same as current
    // invalidated the rated and declined flags
    if (trackVersion) {

        NSString *version = [NSBundle.mainBundle.infoDictionary objectForKey:(NSString *)kCFBundleVersionKey];

        // the already rated version
        NSString *rated_version = @"";

        if ([defs objectForKey:@key_track_version] != nil)
            rated_version = [defs stringForKey:@key_track_version];

        if (![rated_version isEqualToString:version]) {
            if (rated) {
                rated = false;
                [defs setBool:rated forKey:@key_rated];
            }

            if (declined) {
                declined = false;
                [defs setBool:declined forKey:@key_declined];
            }

            [defs setObject:version forKey:@key_track_version];

            // restart the launch count
            [defs setInteger:1 forKey:@key_launch_count];
            [defs setDouble:[[NSDate date] timeIntervalSince1970] forKey:@key_launch_date];

            //[defs synchronize];

            return true;
        }
    }

    if (rated || declined)
        return true;
    return false;
}

+ (bool)acted
{
    return [ManglRateAnnoyer.instance acted_internal];
}


- (void)launchInternal
{
    if (debug) {
        [self performSelectorOnMainThread:@selector(showPrompt:) withObject:nil waitUntilDone:false];
        return;
    }

    if ([self acted_internal])
        return;

    auto defs = NSUserDefaults.standardUserDefaults;

    bool need_rate = false;

    //
    // handle launch counts condition
    //
    int launch_count = 0;

    if ([defs objectForKey:@key_launch_count] != nil)
        launch_count = (int)[defs integerForKey:@key_launch_count];

    if (launch_count >= launchesToRate) {
        need_rate = true;
    } else {
        launch_count++;
        [defs setInteger:launch_count forKey:@key_launch_count];
    }

    //
    // handle the launch date condition
    //
    NSTimeInterval first_launch = 0;

    if ([defs objectForKey:@key_launch_date] != nil)
        first_launch = [defs doubleForKey:@key_launch_date];

    if (first_launch == 0) {
        first_launch = [[NSDate date] timeIntervalSince1970];
        [defs setDouble:first_launch forKey:@key_launch_date];
        return;
    }

    // check the launch of the current version

    // get current date
    auto current_time = [[NSDate date] timeIntervalSince1970];
    if ((current_time - first_launch) / (60 * 60 * 24) > daysToRate)
        need_rate = true;

    // check if the network is available
    if (need_rate) {
        if (NotReachable == ManglReachability.reachabilityForInternetConnection.currentReachabilityStatus)
            need_rate = false;
    }

    if (need_rate) {
        [self showPrompt:self];
    }

    //[defs synchronize];
}

+ (void)launch
{
#if defined(MANGL_APP_ID)
    [ManglRateAnnoyer.instance launchInternal];
#endif
}

- (void)rate_internal
{
    auto defs = NSUserDefaults.standardUserDefaults;

    [defs setBool:true forKey:@key_rated];
    //[defs synchronize];

#if APEAL_OS_IPHONE

//    double versionValue = UIDevice.currentDevice.systemVersion.floatValue;

//    if (versionValue >= 8.0) {
    // TODO: TEST: itms://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=
    NSString *rate_url_template =
        @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/"
        @"viewContentsUserReviews?id=%@&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software";
    self.ratingURL = [NSString stringWithFormat:rate_url_template, _appId];

//    } else {
//
//        // Changes in iOS 6
//        if (versionValue >= 6.0) {
//            NSString *rate_url_template = @"itms-apps://itunes.apple.com/%@/app/id%@";
//            self.ratingURL =
//                [NSString stringWithFormat:rate_url_template, [NSLocale.currentLocale objectForKey:NSLocaleCountryCode], _appId];
//        } else {
//            NSString *rate_url_template =
//                @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/"
//                @"viewContentsUserReviews?id=%@&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software";
//            self.ratingURL = [NSString stringWithFormat:rate_url_template, _appId];
//        }
//    }

    auto nsurl = [NSURL URLWithString:self.ratingURL];

    [UIApplication.sharedApplication openURL:nsurl options:@{} completionHandler:^(BOOL success) {
    }];

#else
    NSString *rate_url_template = @"macappstore://itunes.apple.com/app/id%@";
    self.ratingURL = [NSString stringWithFormat:rate_url_template, _appId];

    [NSWorkspace.sharedWorkspace openURL:[NSURL URLWithString:self.ratingURL]];
    [self openAppPageWhenAppStoreLaunched];
#endif
}

+ (void)rate
{
    [ManglRateAnnoyer.instance rate_internal];
}



- (void)clear
{
    auto defs = NSUserDefaults.standardUserDefaults;
    [defs removeObjectForKey:@key_rated];
    [defs removeObjectForKey:@key_launch_date];
    [defs removeObjectForKey:@key_launch_count];
    [defs removeObjectForKey:@key_track_version];
    [defs removeObjectForKey:@key_declined];
    //[defs synchronize];
}


@end

#endif  // If rate annoyer is enabled

