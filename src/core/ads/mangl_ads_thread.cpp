/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_ads_client.h"

#if MANGL_ADS && MANGL_ADS_EMBED

//#include <thread>
//#include <chrono>
//
//
//void MAdsNetworkClient::bannerThreadBody()
//{
//	using namespace std ;
//
//	for(;;) {
//		if ( hidden_ ) {
//			this_thread::sleep_for(chrono::seconds(1)) ;
//			continue ;
//		}
//		
//		try {
//			Ad ad ;
//			if ( selectAd(ad) ) {
//				launchAd(ad) ;
//			} else {
//				removeAd() ;
//			}
//
//			this_thread::sleep_for(chrono::seconds(30)) ;
//		}
//		catch(...) {
//		}
//	}
//}

#endif
