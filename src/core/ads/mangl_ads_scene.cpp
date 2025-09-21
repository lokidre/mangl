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

#include "../../../environment.h"
#include "../animation/mangl_animation.h"
#include "../game/mangl_framework.h"
#include "../render/mangl_render.h"
#include "../scene/mangl_scene.h"
#include "../widgets/mangl_button.h"

#include <apeal/parse.hpp>


void MAdsNetworkClient::setRender(MSceneRender *render)
{ 
	ctx_.scene = render->newScene() ;
}


void MAdsNetworkClient::loadLaunchAd()
{
	using namespace apl ;
	
	auto &ad = launchAd_ ;
	const auto &rec = database_[ad.databaseIndex] ;
	
	auto scene = ctx_.scene ;
	
	
	scene->mutex_.lock() ;
	
	//
	// First, clear the scene
	//
	scene->clear() ;
	
	ctx_.scene->enableLock(true) ;

	ctx_.scene->frameSet_ = true ;
	ctx_.scene->frame_ = safeLayout ;
	ctx_.scene->setScissor(safeLayout) ;
	
	
	auto &ctx = ctx_ ;
	auto ani = ctx.animation ;
	auto adDir = adsDir_ + "/" + rec.dir ;
	auto anipath = adDir + "/" + rec.ani ;
	
	//debug_printf ( "Ads: loading %s", anipath.c_str() ) ;
	
	ani->clear() ;
	ani->setViewport ( scene->frame_ ) ;
	ani->preload(anipath) ;
	if ( !rec.tex.empty() )
		render_->loadAtlas ( adDir + "/" + rec.tex, &ctx.tex, ctx.textureContext ) ;
	
	// adjust the ad
	auto screenLayout = ani->layoutFlag ? ani->screenLayout : scene->frame_.size() ;
	auto viewport = scene->frame_ ;
	auto scale = viewport.h/screenLayout.h ;
	if ( screenLayout.w == 0 )
		screenLayout.w = viewport.w / scale ;
	viewport.x = (viewport.w - screenLayout.w*scale)/2 ;

	
	
	ani->clear() ;
	ani->setParameter("BANNER",1) ;
	//ani->setParameters(params) ;
	ani->setViewport(viewport) ;
	ani->setScale(scale) ;
	ani->load(anipath) ;
	ani->create ( scene, &ctx.tex ) ;
	ani->start(timestamp_) ;
	
	
	//
	// create gwidget
	//
	scene->newWidget<Button>(M::VISIBLE)->move({0,0,viewport.w,viewport.h})->connect ( this, &MAdsNetworkClient::onBannerPressed ) ;
	
	
	
	/*
		//
		// sample
		//
		static float b = 0 ;
		b = ( b == 0 ) ? 1.0f : 0 ;
	 
		scene->newSprite(M::VISIBLE)->setRect({0,0,scene->frame_.w,scene->frame_.h})->setFillColor({1.0f,0,b,1.f}) ;
		scene->newSprite(M::VISIBLE)->setRect({0,40,scene->frame_.w,scene->frame_.h})->setFillColor({1.0f,1.f,b,1.f}) ;
		*/
	
	scene->mutex_.unlock() ;
}


void MAdsNetworkClient::startTransition()
{
	transitionOutActive_ = true ;
	transitionOutBeginTimestamp_ = timestamp_ ;
	transitionOutEndTimestamp_ = timestamp_ + currentTransitionTime_ ;

	currentTransition_ = (Transition)mangl_framework_->rand(TRANSITIONS_COUNT) ;
	//currentTransition_ = TRANSITION_ANIMATE_ROTATE_Y ;
	currentTransitionTime_ = 0.7f ;


	ctx_.scene->viewMatrix_ = &transitionMatrix_ ;
	transitionMatrix_.eye() ;
}

void MAdsNetworkClient::transitionOutTimer()
{
	if ( timestamp_ >= transitionOutEndTimestamp_ ) {
		transitionPhase(0) ;
		loadLaunchAd() ;
		
		transitionOutActive_ = false ;
		
		ctx_.scene->viewMatrix_ = &transitionMatrix_ ;
		transitionMatrix_.eye() ;

		transitionInActive_ = true ;
		transitionInBeginTimestamp_ = timestamp_ ;
		transitionInEndTimestamp_ = timestamp_ + currentTransitionTime_ ;
		
		return  ;
	}
	
	Real lambda = (timestamp_ - transitionOutBeginTimestamp_)/(transitionOutEndTimestamp_-transitionOutBeginTimestamp_) ;

	transitionPhase(1 - lambda) ;
}

void MAdsNetworkClient::transitionInTimer()
{
	if ( timestamp_ >= transitionInEndTimestamp_ ) {
		transitionInActive_ = false ;
		transitionPhase(1.0f) ;

		ctx_.scene->viewMatrix_ = 0 ;
		ctx_.scene->setScissor(safeLayout) ;
		return  ;
	}
	
	float lambda = (timestamp_ - transitionInBeginTimestamp_)/(transitionInEndTimestamp_-transitionInBeginTimestamp_) ;
	
	transitionPhase(lambda) ;
}

void MAdsNetworkClient::transitionPhase(float lambda)
{
	transitionMatrix_.eye() ;
	
	auto screenLayout = safeLayout ;
	if ( mangl_environment_->isStretch )
		screenLayout.scale(mangl_environment_->stretchValue) ;

	switch ( currentTransition_ ) {
		case TRANSITION_ANIMATE_SIDE :
		{
			float offset = -(1.f-lambda) * safeLayout.w ;
			transitionMatrix_.translate ( offset, 0, 0 ) ;
			ctx_.scene->setScissor({ 0, 0, safeLayout.w + offset, safeLayout.h }) ;
			break ;
		}
		case TRANSITION_ANIMATE_VERTICAL : 
		{
			float offset = -(1.f-lambda) * safeLayout.h ;
			transitionMatrix_.translation ( 0, offset, 0 ) ;
			ctx_.scene->setScissor({ 0, 0, safeLayout.w, safeLayout.h + offset }) ;
			break ;
		}
		case TRANSITION_ANIMATE_ZOOM : 
		{
			float x = (1.0f - lambda) * screenLayout.w/2 ;
			float y = (1.0f - lambda) * screenLayout.h/2 ;
			transitionMatrix_.translation ( x, y, 0 ) ;
			transitionMatrix_.zoom ( lambda ) ;
			ctx_.scene->setScissor({ x, y, safeLayout.w*lambda, safeLayout.h*lambda}) ;
			break ;
		}
		case TRANSITION_ANIMATE_ZOOM_ROTATE_Z : 
		{
			float rho = cognit::const_pi<float>()() * 2 * (1.0f - lambda ) ;

			Matrix R, U, V, Z ;
			
			Z.eye() ;
			Z.zoom(lambda) ;

			R.rotationZ(rho) ;

			U.translation(screenLayout.w/2,screenLayout.h/2,0) ;
			V.translation(-screenLayout.w/2,-screenLayout.h/2,0) ;

			transitionMatrix_ = U * R * Z * V ;
			break ;
		}

		case TRANSITION_ANIMATE_ROTATE_Y : 
		{
			float rho = cognit::const_pi<float>()() / 2 * (1.0f - lambda ) ;

			Matrix R, U, V ;

			R.rotationY(rho) ;
			U.translation(screenLayout.w/2,0,0) ;
			V.translation(-screenLayout.w/2,0,0) ;

			transitionMatrix_ = U * R * V ;
			break ;
		}
		case TRANSITION_FADE :
			ctx_.scene->setAlpha(lambda) ;
			break ;
		default :
			break ;
	}
}

#endif

#endif
