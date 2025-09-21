#import "mangl_google_signin.h"

#import "mangl_main_vc.h"
#import "mangl_invite.h"

#include "../core/mangl_framework.h"


#if MANGL_GOOGLE_SIGNIN

#import <Firebase/Firebase.h>


static bool manglGoogleSignInTableItem_ = manglPushInit([]{
    [MManglGoogleSignIn.instance onLaunch];
});


@implementation MManglGoogleSignIn

+ (instancetype)instance {
    static MManglGoogleSignIn *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[MManglGoogleSignIn alloc] init];
    });

    return sharedInstance ;
}


-(void)onLaunch
{
    if ( mangl_environment_->firebaseEnabled ) {
        GIDSignIn.sharedInstance.clientID = FIRApp.defaultApp.options.clientID;
        GIDSignIn.sharedInstance.delegate = self;
        [GIDSignIn.sharedInstance restorePreviousSignIn];
    }
}

//
// MARK: GIDSignInDelegate
//
- (void)signIn:(GIDSignIn *)signIn didSignInForUser:(GIDGoogleUser *)user withError:(NSError *)error
{
	if ( error )
		return ;

	auto authentication = user.authentication;
	auto credential = [FIRGoogleAuthProvider credentialWithIDToken:authentication.idToken accessToken:authentication.accessToken] ;

#if MANGL_FIREBASE_INVITES
	auto invite = MInvite.instance ;
	if ( invite.firebaseInvitePending ) {
		invite.firebaseInvitePending = false ;
		[MInvite.instance showFirebaseInviteDialog:invite.firebaseInviteMessage withTitle:invite.firebaseInviteTitle] ;
	}
#endif
}

- (void)signIn:(GIDSignIn *)signIn didDisconnectWithUser:(GIDGoogleUser *)user withError:(NSError *)error {
// Perform any operations when the user disconnects from app here.
}


@end

#endif





