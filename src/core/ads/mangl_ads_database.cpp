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

#if 0

#include <algorithm>
#include <numeric>
#include <regex>

#include "../../../environment.h"
#include "../game/mangl_framework.h"
#include "../game/mangl_random.h"


#include <apeal/crypt/crutil.hpp>
#include <apeal/parse.hpp>
#include <apeal/tar.hpp>
#include <apeal/zlib.hpp>


void MAdsNetworkClient::initDatabase()
{
	using namespace apl ;

	auto bundleDir = mangl_environment_->bundle_dir ;

	File<> file ;
	AFileSystem<> fs ;

	if ( !fs.exist(adsDir_) )
		fs.mkdir ( adsDir_, fs.mkdir_parents ) ;

	auto zips = fs.readDir(bundleDir,"banner-*.tar.gz") ;

	for ( auto &z : zips ) {
		auto filepath = fs.makeFilePath ( bundleDir, z.name ) ;

		// compute zip's md5
		auto md5 = file_md5(filepath) ;

		auto tarname = fs.removeExt(z.name) ;
		auto addir = fs.removeExt(tarname) + ".ad" ;
		auto adpath = fs.makeFilePath(adsDir_,addir) ;
		auto tarpath = fs.makeFilePath(adpath,tarname) ;
		auto md5path = fs.makeFilePath(adpath,"md5") ;


		// if file directory already exists, continue
		bool needad = false ;
		if ( !fs.exist(adpath) ) {
			// extract the zip archive into this dir
			fs.mkdir(adpath) ;
			needad = true ;
		} else {
			if ( !fs.exist(md5path) ) {
				needad = true ;
			} else {
				std::string stored_md5 ;
				file.load ( md5path, stored_md5 ) ;
				if ( stored_md5 != md5 )
					needad = true ;
			}
		}
		
		string_type adfile ;

		if ( needad ) {
			// Uncompress the gzip file
			AZLib<> zlib ;
			zlib.inflateGzipFile ( filepath, tarpath ) ;

			// Untar the contents
			ATar<> tar ;
			tar.extract ( tarpath, adpath ) ;
			
			// select the correct file
			regex banner_regex(R"(banner-.+-ad\.ini)") ;
			for ( auto &fname : tar.files ) {
				if ( regex_match(fname,banner_regex) ) {
					adfile = fname ;
					break ;
				}
			}

			// remove the tar file
			fs.rm(tarpath) ;

			file.save ( md5path, md5 ) ;
			
		} else {

			// Read the banner information file
			auto bannerFiles = fs.readDir ( adpath, "banner-*-ad.ini" ) ;
			if ( !bannerFiles.empty() )
				adfile = bannerFiles[0].name ;
		}
		
		if ( !adfile.empty() ) {
			DatabaseRecord r ;
			r.dir = addir ;
			r.ini = adfile ;

			//auto adDir = adsDir_ + "/" + ad.dir ;
			auto iniFile = adsDir_ + "/" + r.dir + "/" + r.ini ;
			bool skip = false ;

			try {
				// parse the banner file
				AIniParser<> iniParser ;
				AIni<> ini ;
				iniParser.load(iniFile,ini) ;
				for ( auto &sec : ini.sec ) {
					if ( "BANNER" == sec.name ) {
						for ( auto &el : sec.el ) {
							if ( "animation" == el.name ) r.ani = el.value ; else
							if ( "texture" == el.name ) r.tex = el.value ; else
							if ( "priority" == el.name ) parse_int(el.value,r.priority) ; else
#ifdef MANGL_APP_ID
							if ( "exclude" == el.name ) {
								vector<MString> v ;
								str_parse(el.value,',',v) ; 
								for ( const auto &id : v ) {
									if ( str_trim(id) == MANGL_APP_ID )
										skip = true ;
								}
							} else
#endif
							#if APEAL_OS_IPHONE
							if ( "ios-appid" == el.name ) ctx_.appid = el.value ; else
							if ( "ios-link" == el.name ) ctx_.applink = el.value ; else
							#elif APEAL_OS_MAC
							if ( "osx-appid" == el.name ) ctx_.appid = el.value ; else
							if ( "osx-link" == el.name ) ctx_.applink = el.value ; else
							#endif
								;
						}
					}
				}

				if ( skip )
					continue ;
				
				if ( r.priority == 0 )
					r.priority = 1 ;

				database_.push_back(r) ;
			}
			catch(exception &e) {
				debug_printf("%s:%d:%s",__FILE__,__LINE__,e.what()) ;
			}
			catch(...) {
				debug_printf("%s:%d:Unhandled Exception!",__FILE__,__LINE__) ;
			}
		}

	}
	
	
	// Clear the queue
	adQueueIndex_ = nextQueueIndex_ = AD_QUEUE_SIZE ;
}

void MAdsNetworkClient::buildAdQueue()
{
#if MANGL_ADS_BANNER_NETWORK
	int embedAdsCount = AD_QUEUE_SIZE/5 ; //mangl_framework_->rand(40,70) ;
#else
	int embedAdsCount = AD_QUEUE_SIZE ;
#endif

	int totalPriority = std::accumulate ( database_.begin(), database_.end(), 0, [](int s, const DatabaseRecord &b ) { return s + b.priority ; } ) ;
	float k = float(totalPriority)/float(embedAdsCount) ;
	for ( auto &r : database_ )
		r.count = int(float(r.priority) / k) ;
	
	// TODO: several embedded ads
	// embed

	size_t queueIndex = 0 ;
	size_t databaseIndex = 0 ;
	for ( queueIndex = 0 ; queueIndex < AD_QUEUE_SIZE ; ++queueIndex ) {
		
		auto &q = adQueue_[queueIndex] ;
		//auto &ed = database_[databaseIndex] ;
		
		if ( ( queueIndex % 5 ) == 0 ) {
			q.databaseIndex = databaseIndex ;
			q.yield = false ;
		} else {
			//q.databaseIndex = database_.size()-1 ;
			q.yield = true ;
		}
	}
	
	
	/*
	for ( auto &r : database_ ) {
		for ( int i = 0 ; i < r.count ; ++i, ++queueIndex ) {
			adQueue_[queueIndex].databaseIndex = databaseIndex ;
			adQueue_[queueIndex].yield = false ;
		}
		++databaseIndex ;
	}
	
	for ( ; queueIndex < embedAdsCount ; ++queueIndex ) {
		adQueue_[queueIndex].databaseIndex = database_.size()-1 ;
		adQueue_[queueIndex].yield = false ;
	}
	
	for ( int i = embedAdsCount ; i < AD_QUEUE_SIZE ; ++i ) {
		adQueue_[i].yield = true ;
	}
	
	// Always keep the first ad - internal
	std::shuffle ( adQueue_.begin()+1, adQueue_.end(), *MRandom::instance() ) ;
	*/
	
	adQueueIndex_ = 0 ;
	nextQueueIndex_ = 0 ;
}


bool MAdsNetworkClient::selectAd(Ad &ad)
{
	adQueueIndex_ = nextQueueIndex_ ;
	
	if ( nextQueueIndex_ >= AD_QUEUE_SIZE ) {
		buildAdQueue() ;
	} else {
		++nextQueueIndex_ ;
	}
	
	auto &queueItem = adQueue_[adQueueIndex_] ;
	
	if ( queueItem.yield ) {
		mangl_framework_->pushFrameworkAction(MANGL_ACTION_ADS_BANNER_YIELD) ;
		return false ;
	}
	
	mangl_framework_->pushFrameworkAction(MANGL_ACTION_ADS_BANNER_ACQUIRE) ;
	
	ad.databaseIndex = queueItem.databaseIndex ;

	return true ;
}

#endif

#endif


