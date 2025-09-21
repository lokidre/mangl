#import "mangl_main_vc.h"

#import "mangl_main_app.h"

#include "../core/audio/mangl_audio_system.h"

#import "../ios/mangl_main_vc+except.h"

#include "mangl_ios_static.h"

@import Foundation;
@import AVFoundation;


#ifdef MANGL_IOS_LAUNCH_VIEW_INCLUDE
#include MANGL_IOS_LAUNCH_VIEW_INCLUDE
#endif

#include "mangl_hooks_ios.h"

#include "../apple/mangl_imacos_util.h"
#include "../apple/mangl_rate_annoyer.h"
#include "../apple/mangl_reachability.h"

#include "../core/mangl_framework.h"

namespace mangl {
void Framework::finish(){
//    [UIApplication.sharedApplication performSelector:@selector(suspend)];
    exit(0);
}
}  // namespace mangl

using namespace mangl;

namespace mangl::internal {
using RuntimeInitTable = std::vector<std::function<void()>>;
static RuntimeInitTable* manglRuntimeInitTable_{};
}  // namespace mangl::internal

bool manglPushInit(std::function<void()> f) {
    using namespace mangl::internal;
    
    if (!manglRuntimeInitTable_)
        manglRuntimeInitTable_ = new RuntimeInitTable;
    
    manglRuntimeInitTable_->push_back(f);
    
    return true;
}


static ManglMainViewController *manglRuntimeViewControllerSharedInstance__{};

@interface ManglMainViewController ()
@end


@implementation ManglMainViewController

+ (instancetype)instance {
    return manglRuntimeViewControllerSharedInstance__;
}


//
// The only point of creation
//
- (id)initWithViewController:(UIViewController *)parent {
    self = [super initWithNibName:nil bundle:nil];
    manglRuntimeViewControllerSharedInstance__ = self;
    [self ctor];

    return self;
}


- (void)ctor {
    viewCreated_ = false;
    applicationActivated_ = false;
    firstAfterLoadView_ = true;
    screenSize_ = {};
    scaleFactor_ = 1;

    scorePosted_ = false;

    wasMemoryWarning_ = false;

//#if MANGL_ADS
//    [self onAdsInit];
//#endif


    [self onExceptInit];
    [self onTimerInit];
    [self onTouchesInit];
//    [self onMusicInit];

    
    // Initialize the components
    using namespace mangl::internal;
    
    if (manglRuntimeInitTable_) {
        for (auto &f: *manglRuntimeInitTable_)
            f();
        delete manglRuntimeInitTable_;   // free some memory
    }
}



- (void)createView {
    firstAfterLoadView_ = true;
    screenSize_ = Env::applicationRect.size();

    // preparing the rendering stuff
    auto viewRect = CGRectMake(0, 0, screenSize_.w, screenSize_.h);
    
    _renderView = [[ManglSceneView alloc] initWithFrame:viewRect];
    _renderView.multipleTouchEnabled = true;
    self.view = _renderView;

    scaleFactor_ = Env::pixelDensity;
    
    [_renderView setOpaque:YES];
    [_renderView setBackgroundColor:nil];
    [_renderView setContentScaleFactor:Env::physScreenSize.w / viewRect.size.width];


    // TODO: implement resize view (for desktop mac)
    [_renderView attachRender:Framework::instance()->getRenderer()];

    viewCreated_ = true;
}

- (void)onApplicationDidBecomeActive
{
    MDebugFunc();

    // Handle audio
    NSError *error{};
    auto result = [AVAudioSession.sharedInstance setActive:YES
                                                     error:&error];
    if (!result) {
        MDebugFunc("AVAudioSession Error: %s", error);
    }


    using namespace impl_;
    auto fw = Framework::instance();

    if (firstAfterLoadView_) {
        fw->onStart();
        fw->onViewport();

        [self startTimer];
        
        fw->processTimer(currentTimestamp_);
        fw->render_->onTimer(currentTimestamp_);

        
        [self onDisplayLink:self];   // Immediately render something

        firstAfterLoadView_ = false;
    } else {
        [self startTimer];
    }

#if MANGL_RATE_ANNOYER
    [ManglRateAnnoyer launch];
#endif

    fw->onResume();

    timerJustEnabled_ = true;
    applicationActivated_ = true;

    AudioSystem::onBecomeActive();
//    [self onMusicDidBecomeActive];
    
#if MANGL_ADS
    [self onApplicationDidBecomeActiveAds];
#endif
}


- (void)onApplicationWillResignActive {
    MDebugFunc();

    AudioSystem::onResignActive();

    // Handle audio
    // Delay execution for .5 second
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC / 2), dispatch_get_main_queue(), ^{
        // Launch the deactivation with some delay
        NSError *error{};
        auto result = [AVAudioSession.sharedInstance setActive:NO
                                                   withOptions:AVAudioSessionSetActiveOptionNotifyOthersOnDeactivation
                                                         error:&error];
        if (!result) {
            MDebugFunc("%s", error);
        }
    });

//    [self onMusicWillResignActive];

    impl_::timerEnabled_ = false;

    Framework::instance()->onResignActive();
    
#if MANGL_ADS
    [self onApplicationWillResignActiveAds];
#endif
//    [self killTimer];
}




// UIViewController
- (void)loadView {
    MDebugFunc();
    [self createView];
}

// UIViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    
    MDebugFunc();
    
#if MANGL_MULTIPLAYER
    self.locationManager = [[CLLocationManager alloc] init];
    self.locationManager.delegate = self;
#endif
    

#if MANGL_ADS
    [self viewDidLoadAds];
#endif
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    
#if MANGL_DEBUG_CRASH
#endif
    
    // Crashlytics test crash
//#if _DEBUG
////    @[][1];
//    int *a = nil; *a = 0;
//#endif

    auto scale = Env::pixelDensity;
    auto bottom = (Cord)self.view.safeAreaInsets.bottom;
    auto top = (Cord)self.view.safeAreaInsets.top;
    auto left = (Cord)self.view.safeAreaInsets.left;
    auto right = (Cord)self.view.safeAreaInsets.right;


    Env::screenPadding = {};
    Env::screenPadding.b = bottom * scale;
    Env::screenPadding.t = top * scale;
    Env::screenPadding.l = left * scale;
    Env::screenPadding.r = right * scale;
    
    // If the bottom is zero, then assume that's it's the rectangular screen
    if (Env::screenPadding.b == 0)
        Env::screenPadding.t = 0;
    
    // iPads report only bottom rounded corners
    if (Env::screenPadding.b != 0 && Env::screenPadding.t == 0)
        Env::screenPadding.t = Env::screenPadding.b;
    
#if MANGL_ADS
    [self viewDidAppearAds];
#endif

}



// UIViewController
- (void)dealloc {
    MDebugFunc();

//    [self killTimer];

    [NSNotificationCenter.defaultCenter removeObserver:self];

    Framework::instance()->onRelease();
}

// UIViewController
- (BOOL)prefersStatusBarHidden {
    return YES;
}

// UIViewController
- (UIStatusBarStyle)preferredStatusBarStyle {
    return UIStatusBarStyleLightContent;
}

#if !MANGL_ORIENTATION_PORTRAIT && !MANGL_ORIENTATION_LANDSCAPE
#define MANGL_ORIENTATION_PORTRAIT 1
#endif

// UIViewController
- (BOOL)shouldAutorotate {
    return NO;
}

// UIViewController
- (UIInterfaceOrientationMask)supportedInterfaceOrientations {
#if MANGL_ORIENTATION_LANDSCAPE
    return UIInterfaceOrientationMaskLandscapeLeft | UIInterfaceOrientationMaskLandscapeRight;
#endif
    return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
}

// UIViewController
//- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
//{
//	[super didRotateFromInterfaceOrientation:fromInterfaceOrientation] ;
//}

//
// Block memory warning. We don't want any view to be released, we didn't allocate many anyway
//
// UIViewController
- (void)didReceiveMemoryWarning
{
    MDebugFunc();

    wasMemoryWarning_ = true;
    [super didReceiveMemoryWarning];
}


#if MANGL_MULTIPLAYER
- (void)requestLocationPermissions {
    if (CLLocationManager.locationServicesEnabled) {
        [_locationManager requestWhenInUseAuthorization];
    } else {
        NSLog(@"Location services are disabled.");
    }
}

// MARK: CLLocationManagerDelegate
- (void) locationManagerDidChangeAuthorization:(CLLocationManager *) manager {
    switch (manager.authorizationStatus) {
        case kCLAuthorizationStatusNotDetermined:
            NSLog(@"User has not determined location access.");
            break;
        case kCLAuthorizationStatusRestricted:
            NSLog(@"Location access is restricted (e.g., parental controls).");
            break;
        case kCLAuthorizationStatusDenied:
            NSLog(@"User denied location access.");
            break;
        case kCLAuthorizationStatusAuthorizedWhenInUse:
            NSLog(@"Location access granted for foreground.");
            [self.locationManager startUpdatingLocation];
            break;
        case kCLAuthorizationStatusAuthorizedAlways:
            NSLog(@"Location access granted for foreground and background.");
            [self.locationManager startUpdatingLocation];
            break;
        default:
            break;
    }
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error {
    NSLog(@"Failed to get location: %@", error.localizedDescription);
}

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray<CLLocation *> *)locations {
    CLLocation *currentLocation = [locations lastObject];
    NSLog(@"Current location: Latitude: %f, Longitude: %f",
          currentLocation.coordinate.latitude,
          currentLocation.coordinate.longitude);
}

#endif


@end
