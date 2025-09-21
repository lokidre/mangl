#import "mangl_invite.h"

#include "../apple/mangl_imacos_util.h"

#import "../ios/mangl_main_vc.h"
#import "../ios/mangl_main_vc+except.h"


#include <apeal/debug.hpp>

using namespace mangl;

@implementation ManglInvite

+ (instancetype)instance {
    static ManglInvite* sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ManglInvite alloc] init];
        [sharedInstance ctor];
    });

    return sharedInstance;
}


- (void)ctor {
}


- (void)inviteSMS:(InviteParamsA)params
{
    if ([MFMessageComposeViewController canSendText]) {
        auto vc = [[MFMessageComposeViewController alloc] init];
        vc.messageComposeDelegate = self;
        
        auto message = toNSString(params.message);
        vc.body = message;

        [ManglMainViewController.instance presentViewController:vc animated:YES completion:nil];
    } else {
        [ManglMainViewController showAlert:@"Messaging Service is not available."];
    }
}


- (void)inviteEmail:(InviteParamsA)params
{
    if ([MFMailComposeViewController canSendMail]) {
        auto vc = [[MFMailComposeViewController alloc] init];
        vc.mailComposeDelegate = self;
        
        auto subject = toNSString(params.title);
        auto message = toNSString(params.message);

        [vc setSubject:subject];
        [vc setMessageBody:message isHTML:NO];

        // Present the view controller modally.
        [ManglMainViewController.instance presentViewController:vc animated:YES completion:nil];
    } else {
        [ManglMainViewController showAlert:@"E-Mail Service is not available."];
    }
}


- (void)inviteFirebase:(NSString *)message withTitle:(NSString *)title
{
#if MANGL_FIREBASE_INVITES
    if (GIDSignIn.sharedInstance.hasPreviousSignIn) {
        [self showFirebaseInviteDialog:message withTitle:title];
    } else {
        _firebaseInviteMessage = message;
        _firebaseInviteTitle = title;
        _firebaseInvitePending = true;
        GIDSignIn.sharedInstance.delegate = self;
        [GIDSignIn.sharedInstance signIn];
    }
#endif
}



//
// MARK: MFMessageComposeViewControllerDelegate
//
- (void)messageComposeViewController:(MFMessageComposeViewController *)controller 
                 didFinishWithResult:(MessageComposeResult)result {
    [ManglMainViewController.instance dismissViewControllerAnimated:YES completion:nil];
}

//
// MARK: MFMailComposeViewControllerDelegate
//
- (void)mailComposeController:(MFMailComposeViewController *)controller
          didFinishWithResult:(MFMailComposeResult)result
                        error:(NSError *)error
{
    [ManglMainViewController.instance dismissViewControllerAnimated:YES completion:nil];
}


// MARK: GIDSignInUIDelegate Protocol
#if MANGL_GOOGLE_SIGNIN

- (void)signInWillDispatch:(GIDSignIn *)signIn error:(NSError *)error
{
}

- (void)signIn:(GIDSignIn *)signIn didSignInForUser:(GIDGoogleUser *)user withError:(NSError *)error
{
}


- (void)signIn:(GIDSignIn *)signIn presentViewController:(UIViewController *)viewController
{
    [MMainViewController.instance presentViewController:viewController animated:YES completion:nil];
}

- (void)signIn:(GIDSignIn *)signIn dismissViewController:(UIViewController *)viewController
{
    [MMainViewController.instance dismissViewControllerAnimated:YES completion:nil];
}


#endif


@end


namespace mangl {

void Invites::inviteSMS(InviteParamsA parm) {
    [ManglInvite.instance inviteSMS:parm];
}

void Invites::inviteEmail(InviteParamsA parm) {
    [ManglInvite.instance inviteEmail:parm];
}

}   // namespace mangl
