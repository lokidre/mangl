/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#import "mangl_main_vc.h"
#include "../core/social/mangl_invites.h"


@import MessageUI;

#if MANGL_GOOGLE_SIGNIN
#import <GoogleSignIn/GoogleSignIn.h>
#endif

@interface ManglInvite: NSObject <MFMailComposeViewControllerDelegate, MFMessageComposeViewControllerDelegate

#if MANGL_GOOGLE_SIGNIN
, GIDSignInDelegate
#endif

>

+(instancetype)instance ;

- (void)inviteSMS:(mangl::InviteParamsA)params;
- (void)inviteEmail:(mangl::InviteParamsA)params;

@end

