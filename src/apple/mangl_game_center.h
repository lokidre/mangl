/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_setup.h"

#if MANGL_GAMECENTER

#import <GameKit/GameKit.h>


@interface ManglGameCenterHelper: NSObject <GKGameCenterControllerDelegate>


+ (instancetype)instance;
//- (id)init;
- (void)onLaunch;


//- (void)launchHomeGui;
//- (void)launchLeaderboardGui:(NSString *)leaderboardId;
//- (void)postScore:(NSString *)leaderboardId score:(int)score;
//- (void)inviteFriends;

@end

#endif
