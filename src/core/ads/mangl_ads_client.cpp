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

namespace mangl {

void EmbedAdsNetworkClient::setup(const AdsSetupParams& params) {
}

}


#if 0

namespace mangl_ads {

void *bannerThreadFunc(void *parm) {  static_cast<MAdsNetworkClient *>(parm)->bannerThreadBody() ; return nullptr ; }
void *serverThreadFunc(void *parm) {  static_cast<MAdsNetworkClient *>(parm)->serverThreadBody() ; return nullptr ; }

}

void MAdsNetworkClient::start()
{
	using namespace apl ;

	//
	// Check and build all the directories
	//
	const auto &cacheDir = mangl_environment_->cacheDir ;

	adsDir_ = cacheDir + "/mangl-ads" ;
	databaseFilePath_ = adsDir_ + "/ads.db" ;


	ctx_.animation = new Animation ;


	//
	// Check and create the database
	//
	initDatabase() ;

	//
	// Start the main thread
	//

	bannerThread_.parm = this ;
	bannerThread_.func = mangl_ads::bannerThreadFunc ;
	bannerThread_.create() ;

	started_ = true ;
}

void MAdsNetworkClient::hideBanner()
{
	hidden_ = true ;
	if ( ctx_.scene )
		ctx_.scene->setVisible(false) ;
}

void MAdsNetworkClient::unhideBanner()
{
	hidden_ = false ;
	if ( ctx_.scene )
		ctx_.scene->setVisible(true) ;
}


void MAdsNetworkClient::onBannerPressed(int)
{
	if ( adQueue_[adQueueIndex_].yield )
		return ;

	mangl_framework_->pushFrameworkAction ( MANGL_ACTION_OPEN_APPSTORE_APP_HOME, ctx_.appid ) ;
}
#endif

#endif
