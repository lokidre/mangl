/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_types.h"
#include "../core/mangl_environment.h"


@import UIKit;

#if MANGL_MULTIPLAYER
@import CoreLocation;
#endif


#import "mangl_scene_view.h"


@interface ManglMainViewController:

#if MANGL_MULTIPLAYER
UIViewController<CLLocationManagerDelegate>
#else
UIViewController
#endif

{

    // Initialization
    bool firstAfterLoadView_;
    bool viewCreated_;
    bool applicationActivated_;

    bool wasMemoryWarning_;

    mangl::Size screenSize_;   // Screen Dimensions
    mangl::Cord scaleFactor_;

    bool scorePosted_;

    //
    // Timer
    //
    mangl::Timestamp fps_, fpsRecip_; //, timestamp_;

    //
    // Music
    //
    bool musicPlaying_, musicPaused_;
    mangl::MusicTrackId musicCurrentId_;

    //
    // Ads
    //
//    bool adActive_;
//    mangl::Rect adsRect_;
}

@property (nonatomic) ManglSceneView *renderView;

//
// Private Properties
//
// Critical Exceptions
@property (copy) NSString *exceptionTitle, *exceptionMessage;

// Timer
@property (nonatomic) CADisplayLink *displayLink;

#if MANGL_MULTIPLAYER
// Location manager
@property (strong, nonatomic) CLLocationManager *locationManager;
#endif



+ (ManglMainViewController *)instance;

- (id)initWithViewController:(UIViewController *)parent;

- (void)onApplicationWillResignActive;
- (void)onApplicationDidBecomeActive;

#if MANGL_MULTIPLAYER
- (void)requestLocationPermissions;
#endif

@end
