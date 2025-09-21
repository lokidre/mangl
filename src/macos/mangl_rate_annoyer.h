/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#if 0
#include <apeal/def.hpp>

#if APEAL_OS_MAC || APEAL_OS_IPHONE

#if APEAL_OS_IPHONE
#import <UIKit/UIKit.h>
#else
#import <Cocoa/Cocoa.h>
#endif


#if APEAL_OS_IPHONE
@interface MRateAnnoyer : NSObject <UIAlertViewDelegate> {
#else
@interface MRateAnnoyer : NSObject <NSAlertDelegate> {
#endif


@public
	NSString *app_id ;
	NSString *app_name ;  // if empty will acquire automatically
	NSString *prompt_title, *prompt_message ;
	NSString *title_button_rate, *title_button_later, *title_button_decline ;

	
	
	int days_to_rate, launches_to_rate ;
	bool debug ;  // debug annoyer default is false
	bool track_version ;  // keep asking for each version
}

+(MRateAnnoyer *)instance ;
+(void)launch ;
+(void)rate ;
+(void)prompt ;
+(bool)acted ;


-(void)clear ;  // clears all the keys, for debugging only

@end

#endif

#endif
    
