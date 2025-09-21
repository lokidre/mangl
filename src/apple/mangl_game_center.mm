#import "mangl_game_center.h"

#import "mangl_gamekit.h"

#include "../ios/mangl_hooks_ios.h"

#include "mangl_imacos_util.h"
#include "../core/mangl_framework.h"
#include "../core/social/mangl_game_center.h"

#if APEAL_OS_IPHONE
#import "../ios/mangl_main_vc.h"
#else
#import "../macos/mangl_main_view.h"
#endif

using namespace mangl;

#if MANGL_GAMECENTER

// MARK: Launch hook
@interface ManglGameCenterLaunchHook: NSObject<ManglIosMainHook>
@end

@implementation ManglGameCenterLaunchHook

- (BOOL)didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [ManglGameCenterHelper.instance onLaunch];
    return true;
}

@end


[[maybe_unused]]
static bool launchHook = []{
    [ManglIosHooks.instance registerHook: [[ManglGameCenterLaunchHook alloc] init]] ;
    return true;
}();

@interface ManglGameCenterHelper() {
    bool authenticationLaunched_;
}

#if APEAL_OS_IPHONE
@property (nonatomic) UIViewController* loginViewController;
#else
@property (nonatomic) NSViewController* loginViewController;
#endif

@end


@implementation ManglGameCenterHelper

+ (instancetype)instance {
    static ManglGameCenterHelper* sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ManglGameCenterHelper alloc] init];
    });

    return sharedInstance;
}

- (id)init {
    self = [super init];
    
    authenticationLaunched_ = false;
    
    _loginViewController = nil;
    
    return self;
}

- (void)onLaunch {
    [self authenticate];
}


- (void)authenticate {
	if (!Env::gamecenterEnabled)
        return;
    
    if (authenticationLaunched_)
        return;
    
    authenticationLaunched_ = true;
    
#if APEAL_OS_IPHONE
    using VC = UIViewController;
#else
    using VC = NSViewController;
#endif
//    auto __weak player = GKLocalPlayer.localPlayer;
    
    GKLocalPlayer.localPlayer.authenticateHandler = ^(VC* vc, NSError* error) {
        self.loginViewController = vc;
            
        if (GKLocalPlayer.localPlayer.isAuthenticated)
            [self onLocalPlayerAuthenticated];

        if (error)
            ADebugPrintFunc("Error: %s", error.localizedDescription);
    };
}

-(void)onLocalPlayerAuthenticated {
#if MANGL_MULTIPLAYER
    [GKLocalPlayer.localPlayer unregisterAllListeners];
    [GKLocalPlayer.localPlayer registerListener:ManglGameKitMatchmaker.instance];
#endif
}


-(void)launchLeaderboardGui:(NSString *)leaderboardId
{
    auto player = GKLocalPlayer.localPlayer;
    if (!player.isAuthenticated) {
        if (self->_loginViewController)
            [ManglMainViewController.instance presentViewController:self->_loginViewController animated:YES completion:nil];
    }
        
    auto gcvc = [[GKGameCenterViewController alloc] init];

    gcvc.gameCenterDelegate = self ;
    gcvc.viewState = GKGameCenterViewControllerStateLeaderboards;
    gcvc.leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;
    gcvc.leaderboardIdentifier = leaderboardId;

#if APEAL_OS_IPHONE
    [ManglMainViewController.instance presentViewController:gcvc animated:YES completion:nil];
#else
    auto sdc = GKDialogController.sharedDialogController;
    sdc.parentWindow = ManglMainView.instance.window;
    [sdc presentViewController:gcvc];
#endif
}

- (void)launchHomeGui {
    [self authenticate];
    
    auto player = GKLocalPlayer.localPlayer;
    if (!player.isAuthenticated) {
        if (_loginViewController) {
            [ManglMainViewController.instance presentViewController:self->_loginViewController 
                                                           animated:YES
                                                         completion:nil];
        } else {
            // TODO: show some alert to the user
        }
        return;
    }
    
    auto gcvc = [[GKGameCenterViewController alloc] init];

    gcvc.gameCenterDelegate = self ;
    gcvc.viewState = GKGameCenterViewControllerStateDefault;
    gcvc.leaderboardTimeScope = GKLeaderboardTimeScopeAllTime;

#if APEAL_OS_IPHONE
    [ManglMainViewController.instance presentViewController:gcvc 
                                                   animated:YES
                                                 completion:nil];
#else
    auto sdc = GKDialogController.sharedDialogController;
    sdc.parentWindow = ManglMainView.instance.window;
    [sdc presentViewController:gcvc];
#endif
}

- (void)postScore:(NSString *)leaderboardId score:(int)score {
    auto scoreReporter = [[GKScore alloc] initWithLeaderboardIdentifier:leaderboardId];
    scoreReporter.value = score;
    [GKScore reportScores:@[scoreReporter] withCompletionHandler:^(NSError *error) {
        //Do something interesting here.
    }];
}

- (void)inviteFriends {
    NSError* error;
    if (@available(iOS 15.0, *)) {
        auto vc = ManglMainViewController.instance;
        [GKLocalPlayer.localPlayer presentFriendRequestCreatorFromViewController: vc error:&error];
    } else {
        // Fallback on earlier versions
        [self launchHomeGui];
    }
}

//
// MARK: GKGameCenterControllerDelegate
//
- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)vc {
#if APEAL_OS_IPHONE
    [ManglMainViewController.instance dismissViewControllerAnimated:YES completion:nil];
#else
    [GKDialogController.sharedDialogController dismiss:self];
#endif
}


@end

namespace mangl {

void GameCenter::authenticate() {
    [ManglGameCenterHelper.instance authenticate];
}

void GameCenter::launchLeaderboardGui(StrView leaderboardId) {
    [ManglGameCenterHelper.instance launchLeaderboardGui:toNSString(leaderboardId)];
}

void GameCenter::launchHomeGui() {
    [ManglGameCenterHelper.instance launchHomeGui];
}

void GameCenter::postScore(StrView leaderboardId, int score) {
    [ManglGameCenterHelper.instance postScore:toNSString(leaderboardId) score:score];
}

void GameCenter::inviteFriends(){
    [ManglGameCenterHelper.instance inviteFriends];
}


}  // namespace mangl

#endif


