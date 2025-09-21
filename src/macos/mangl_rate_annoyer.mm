#if 0

#include "mangl_rate_annoyer.h"

#include "../core/mangl_types.h"

#include "../iosmac/mangl_reachability.h"


#include <apeal/str.hpp>

#define key_rated            "key_rate_rated"			// rated flag (bool)
#define key_launch_date      "key_rate_launch_date"  // first launch date (double)
#define key_launch_count     "key_rate_launch_count"	// launch count (int)
#define key_track_version    "key_rate_track_version"  // the version which is being tracked (string)
#define key_declined         "key_rate_declined"		// user declined to rate (bool)

//MRateAnnoyer *rate_annoyer_instance_ = 0 ;

@interface MRateAnnoyer()
@property (nonatomic, retain) NSString *ratingURL;
@end


@implementation MRateAnnoyer

+(MRateAnnoyer *)instance
{
	static MRateAnnoyer* sharedInstance = nil;
	static dispatch_once_t onceToken;
	dispatch_once(&onceToken, ^{
		sharedInstance = [[MRateAnnoyer alloc] init];
		[sharedInstance ctor] ;
	});
	return sharedInstance;
	
	
	//if ( 0 == rate_annoyer_instance_ ) {
	//	rate_annoyer_instance_ = [[MRateAnnoyer alloc] init] ;
	//	[rate_annoyer_instance_ ctor] ;
	//}
	//return rate_annoyer_instance_ ;
}

-(void)ctor
{
#if defined(MANGL_APP_ID)
	using namespace apl ;
	
	debug = false ;
	
#if MANGL_RATE_ANNOYER_DEBUG
	debug = true ;
#endif
	
	track_version = false ;
	
	app_id = @MANGL_APP_ID ;
	app_name = @MANGL_APP_TITLE ; // [[[NSBundle mainBundle] infoDictionary] objectForKey:(NSString *)kCFBundleNameKey] ;
	prompt_title = [NSString stringWithFormat:@"Rate %@", app_name, nil] ;
	
#ifdef MANGL_RATE_ANNOYER_TITLE
	prompt_title = @MANGL_RATE_ANNOYER_TITLE ;
#endif
	
	prompt_message = [NSString stringWithFormat:@"If you enjoy %@, would you please take a moment to rate it? It won't take much time.\n\nThanks for your support!", app_name, nil ] ;
	
	title_button_rate = [NSString stringWithFormat:@"Rate %@", app_name, nil ] ;

	
#ifdef MANGL_RATE_ANNOYER_RATE_BUTTON
	title_button_rate = @MANGL_RATE_ANNOYER_RATE_BUTTON ;
#endif
	
	
	title_button_later = @"Later" ;
	title_button_decline = @"No, Thanks" ;

	days_to_rate = 2 ;
	launches_to_rate = 16 ;
	
#if MANGL_RATE_ANNOYER_LAUNCHES
	launches_to_rate = MANGL_RATE_ANNOYER_LAUNCHES ;
#endif
	
#if MANGL_RATE_ANNOYER_DAYS
	days_to_rate = MANGL_RATE_ANNOYER_DAYS ;
#endif
	
#endif  // rate annoyer defined
}


-(void)show_prompt:(id)sender
{

#if APEAL_OS_IPHONE
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:prompt_title
                                                    message:prompt_message
                                                   delegate:self
                                          cancelButtonTitle:title_button_decline
                                          otherButtonTitles:title_button_rate,
                          title_button_later, nil];
	[alert show] ;
#else
	NSAlert *alert = [[NSAlert alloc] init] ;
	[alert setShowsHelp:NO] ;
	[alert setAlertStyle:NSAlertStyleInformational];
	[alert setDelegate:self] ;
	[alert setMessageText:prompt_title] ;
	[alert setInformativeText:prompt_message] ;
	[alert addButtonWithTitle:title_button_rate] ;
	[alert addButtonWithTitle:title_button_later] ;
	[alert addButtonWithTitle:title_button_decline] ;
	NSInteger response = [alert runModal];
	switch (response) {
		case NSAlertFirstButtonReturn:
			[self onUserRate];
			break ;
		case NSAlertSecondButtonReturn:
			[self onUserLater];
			break ;
		case NSAlertThirdButtonReturn:
			[self onUserDeclined];
			break ;
	}
#endif
}

+(void)prompt
{
	[[MRateAnnoyer instance] show_prompt:0] ;
}

-(bool)acted_internal
{
	if ( [app_id length] == 0 )
		@throw ([NSException exceptionWithName:@"Rate Annoyer" reason:@"app_id is not set" userInfo:nil]) ;
	
	
    auto defs = NSUserDefaults.standardUserDefaults;
    bool rated{}, declined{};
    
	if ( [defs objectForKey:@key_rated] != nil )
		rated = [defs boolForKey:@key_rated] ;
    
	if ( [defs objectForKey:@key_declined] != nil )
		declined = [defs boolForKey:@key_declined] ;
    
    
	
	// if we are tracking the version, and if the tracking version is not the same as current
	// invalidated the rated and declined flags
	if ( track_version ) {
		
		NSString *version = [NSBundle.mainBundle.infoDictionary objectForKey:(NSString *)kCFBundleVersionKey] ;
		
		// the already rated version
		NSString *rated_version = @"" ;
        
		if ( [defs objectForKey:@key_track_version] != nil )
			rated_version = [defs stringForKey:@key_track_version] ;
        
        if ( ![rated_version isEqualToString:version] ) {
            if ( rated ) {
                rated = false ;
                [defs setBool:rated forKey:@key_rated] ;
            }
            
            if ( declined ) {
                declined = false ;
                [defs setBool:declined forKey:@key_declined] ;
            }
            
            [defs setObject:version forKey:@key_track_version] ;
            
            // restart the launch count
            [defs setInteger:1 forKey:@key_launch_count] ;
            [defs setDouble:[[NSDate date] timeIntervalSince1970] forKey:@key_launch_date] ;
            
            [defs synchronize] ;
            
            return true ;
        }
        
    }
    
    if ( rated || declined )
        return true ;
	return false ;
}

+(bool)acted
{
	return [[MRateAnnoyer instance] acted_internal] ;
}


-(void)launch_internal
{
	if (debug) {
		[self performSelectorOnMainThread:@selector(show_prompt:) withObject:nil waitUntilDone:false];
		return ;
	}
	
	if ( [self acted_internal] )
		return ;
	
	NSUserDefaults *defs = [NSUserDefaults standardUserDefaults] ;

    bool need_rate = false ;
    
	//
	// handle launch counts condition
	//
	int launch_count = 0 ;
    
    if ( [defs objectForKey:@key_launch_count] != nil )
	    launch_count = (int)[defs integerForKey:@key_launch_count] ;
    
	if ( launch_count >= launches_to_rate ) {
		need_rate = true ;
    } else {
		launch_count++ ;
		[defs setInteger:launch_count forKey:@key_launch_count] ;
    }
	
	//
	// handle the launch date condition
	//
	NSTimeInterval first_launch = 0 ;
    
    if ([defs objectForKey:@key_launch_date] != nil)
	    first_launch = [defs doubleForKey:@key_launch_date];
    
	if (first_launch == 0) {
		first_launch = [[NSDate date] timeIntervalSince1970];
		[defs setDouble:first_launch forKey:@key_launch_date];
        return;
	}
    
    // check the launch of the current version
	
	// get current date
	NSTimeInterval current_time = [[NSDate date] timeIntervalSince1970] ;
	if ( (current_time-first_launch)/(60*60*24) > days_to_rate )
		need_rate = true ;
		
	// check if the network is available
	if (need_rate) {
        auto status = ManglReachability.reachabilityForInternetConnection.currentReachabilityStatus;
		if (NotReachable == status)
			need_rate = false;
	}

	if (need_rate) {
		//[self performSelectorOnMainThread:@selector(show_prompt) withObject:nil waitUntilDone:false];
		[self show_prompt:self];
	}

	
	[defs synchronize];
}

+(void)launch
{
#if defined(MANGL_APP_ID)
	[MRateAnnoyer.instance launch_internal] ;
#endif
}


#if APEAL_OS_MAC && !APEAL_OS_IPHONE
- (void)openAppPageWhenAppStoreLaunched
{
	//check if app store is running
	for ( NSRunningApplication *app in NSWorkspace.sharedWorkspace.runningApplications ) {
		if ( [app.bundleIdentifier isEqualToString:@"com.apple.appstore"] ) {
			//open app page
			[NSWorkspace.sharedWorkspace performSelector:@selector(openURL:) withObject:self.ratingURL afterDelay:1.0];
			return;
		}
	}
	
	//try again
	[self performSelector:@selector(openAppPageWhenAppStoreLaunched) withObject:nil afterDelay:0.0] ;
}
#endif

-(void)rate_internal
{
	NSUserDefaults *defs = [NSUserDefaults standardUserDefaults] ;
			
	[defs setBool:true forKey:@key_rated] ;
	[defs synchronize] ;
	
	//NSString *url_string ;

#if APEAL_OS_IPHONE
	double versionValue = UIDevice.currentDevice.systemVersion.floatValue ;

	if ( versionValue >= 8.0 ) {
		// TODO: TEST: itms://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=
		NSString *rate_url_template = @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%@&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software" ;
		self.ratingURL = [NSString stringWithFormat:rate_url_template, app_id] ;
		
	} else {
	
		// Changes in iOS 6
		if ( versionValue >= 6.0 ) {
			NSString *rate_url_template = @"itms-apps://itunes.apple.com/%@/app/id%@" ;
			self.ratingURL = [NSString stringWithFormat:rate_url_template, [NSLocale.currentLocale objectForKey: NSLocaleCountryCode], app_id] ;
		} else {
			NSString *rate_url_template = @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%@&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software" ;
			self.ratingURL = [NSString stringWithFormat:rate_url_template, app_id] ;
		}
	}
	
	[UIApplication.sharedApplication openURL:[NSURL URLWithString:self.ratingURL]] ;
#else
	NSString *rate_url_template = @"macappstore://itunes.apple.com/app/id%@" ;
    self.ratingURL = [NSString stringWithFormat:rate_url_template, app_id] ;
	
	[NSWorkspace.sharedWorkspace openURL:[NSURL URLWithString:self.ratingURL]] ;
	[self openAppPageWhenAppStoreLaunched];
	
#endif
}

+(void)rate
{
	[MRateAnnoyer.instance rate_internal] ;
}

#pragma mark Alert Delegate

-(void)onUserDeclined {
	NSUserDefaults *defs = [NSUserDefaults standardUserDefaults] ;
	[defs setBool:true forKey:@key_declined] ;
	[defs synchronize] ;
}

-(void)onUserLater {
	NSUserDefaults *defs = [NSUserDefaults standardUserDefaults] ;
	// give the user some more time
	[defs setDouble:[[NSDate date] timeIntervalSince1970] forKey:@key_launch_date] ;
	[defs setInteger:launches_to_rate/2 forKey:@key_launch_count] ;
	[defs synchronize] ;
}

-(void)onUserRate {
	[self rate_internal] ;
}

#if APEAL_OS_IPHONE
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
	
	switch ( buttonIndex ) {
	case 0:
		[self onUserDeclined] ;
		break ;
		
	case 1 :  // user decided to rate
		[self onUserRate] ;
		break;
		
	case 2 :  // remind later
		[self onUserLater] ;
		break ;
	default:
		break;
	}
}
#endif

-(void)clear
{
	NSUserDefaults *defs = [NSUserDefaults standardUserDefaults] ;
	[defs removeObjectForKey:@key_rated] ;
	[defs removeObjectForKey:@key_launch_date] ;
	[defs removeObjectForKey:@key_launch_count] ;
	[defs removeObjectForKey:@key_track_version] ;
	[defs removeObjectForKey:@key_declined] ;
	[defs synchronize] ;
}


@end

#endif

