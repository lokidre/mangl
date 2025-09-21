#import "mangl_app_delegate.h"

#import "mangl_main_view.h"
#import "mangl_main_vc.h"
#import "mangl_main_window.h"


#include "../core/audio/mangl_music.h"
#include "../core/mangl_framework.h"

#if MANGL_RATE_ANNOYER
#include "../iosmac/mangl_rate_annoyer.h"
#endif

#if MANGL_ADS
#import "mangl_ads_macos.h"
#endif

#include <apeal/debug.hpp>


using namespace mangl;

@interface ManglAppDelegate()
@property (nonatomic) IBOutlet ManglMainWindowObjC *window;
@end

@implementation ManglAppDelegate

-(id)init
{
    self = [super init];

    mangl_main();
    mangl_framework_->init();

    return self;
}


// MARK: NSApplicationDelegate

-(void)applicationDidFinishLaunching:(NSNotification *)notification
{
#if MANGL_ADS
    [MAdNetworkManager.sharedInstance onApplicationDidFinishLaunching:notification];
#endif

    //
    // Create main window
    //
    //_mainWindowDelegate = [[MMainWindowDelegate alloc] init];
    //[_mainWindowDelegate createMainWindow:_window];

    [self performSelectorOnMainThread:@selector(continueLaunch:) withObject:self waitUntilDone:NO];
}

-(void)continueLaunch:(id)sender
{
    [_window onLaunch];
	
	/* TODO: initialization check
#if MANGL_GAMECENTER
	
	auto env = mangl_environment_ ;
	
	if ( env->gamecenterEnabled ) {
		if ( [GKLocalPlayer class] == nil )
			env->gamecenterEnabled = false ;
	}
	
	// Disable the game center for 10.8.xx version
	// it's buggy and obsolete anyway
	if ( env->osVersionHi == 10 && env->osVersionLo <= 8 )
		env->gamecenterEnabled = false ;
	
	if ( env->gamecenterEnabled ) {
		
		auto localPlayer = GKLocalPlayer.localPlayer ;
		
		localPlayer.authenticateHandler = ^(NSViewController *viewController, NSError *error) {
			self.window.mainView.gcAuthenticationViewController = viewController ;
			
			if ( viewController ) {
				env->gamecenterAuthenticated = false ;
				DLog(@"GameCenter Logged Out") ;
				return ;
			}
			
			if ( GKLocalPlayer.localPlayer.isAuthenticated ) {
				//[self authenticatedPlayer];
				env->gamecenterAuthenticated = true ;
				DLog(@"GameCenter Authenticated") ;
			} else {
				//[self disableGameCenter];
				env->gamecenterAuthenticated = false ;
				DLog(@"GameCenter Logged Out") ;
			}
		} ;
		
		if ( !localPlayer.isAuthenticated ) {
			env->gamecenterAuthenticated = false ;
			DLog(@"GameCenter Logged Out") ;
		}
		
	}

#endif
	 */
	
	
    [_window setIsVisible:YES];

#if MANGL_RATE_ANNOYER
    [ManglRateAnnoyer launch];
#endif
}


- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)app {
    return YES ;
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
    [_window onTerminate];
    return NSTerminateNow;
}

- (void)applicationWillTerminate:(NSNotification *)notification {
    // save current game state and settings
    //mangl_framework_->onTerminate() ;
    //mangl_framework_->render_->release() ;
}



@end
