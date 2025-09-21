/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/singleton.hpp>
#include <apeal/thread.hpp>

#include "../mangl_ads.h"

#include "../mangl_texture_atlas.h"

#include <array>

#if MANGL_ADS && MANGL_ADS_EMBED

namespace mangl {

//struct EmbedAdsNetworkSetupParams {
//};

class EmbedAdsNetworkClient: public apl::Singleton<EmbedAdsNetworkClient> {
public :

    void setup(const AdsSetupParams& params);
    void onTimer(Timestamp timestamp);

    void showBanner(){}
    void hideBanner(){}
    void unhideBanner(){}

    void showInterstitial(){}
    void showInterstitialDelayed(Timestamp){}

    /*
    void setRender ( MSceneRender *render ) ;
    void setLayout ( const MRect &layout ) {    layout_ = layout ; }
    void start() ;
    void stop() ;


private :
    bool started_ = false ;
    MSceneRender *render_{} ;

    MString adsDir_ ;
    MString databaseFilePath_ ;

    MRect layout_ ;
    float timestamp_{} ;

    struct SceneContext {
        MScene *scene{} ;
        Animation *animation{} ;
        MTextureAtlas tex ;
        MTextureContext *textureContext ;

        MString applink, appid ;
    } ctx_ ;

    struct Ad {
        size_t databaseIndex ;
    } ;

    Ad launchAd_ ;
    bool scheduleAdTransition_ = false ;
    bool scheduleAdRemove_ = false ;


private :
    //
    // Scene
    //
    void launchAd(Ad &ad) {
        scheduleAdTransition_ = true ;
        launchAd_ = ad ;
    }

    void removeAd() { scheduleAdRemove_ = true ; }
    void loadLaunchAd() ;
    void buildAdQueue() ;



    enum Transition {
        TRANSITION_FADE,
        TRANSITION_ANIMATE_SIDE,
        TRANSITION_ANIMATE_VERTICAL,
        TRANSITION_ANIMATE_ZOOM,
        TRANSITION_ANIMATE_ZOOM_ROTATE_Z,
        TRANSITION_ANIMATE_ROTATE_Y,

        TRANSITIONS_COUNT
    } ;

    Transition currentTransition_ = TRANSITION_FADE ;
    Timestamp currentTransitionTime_ ;
    Matrix transitionMatrix_ ;

    bool transitionOutActive_ = false, transitionInActive_ = false ;
    Timestamp transitionOutBeginTimestamp_{}, transitionOutEndTimestamp_{} ;
    Timestamp transitionInBeginTimestamp_{}, transitionInEndTimestamp_{} ;

    void startTransition() ;
    void transitionOutTimer() ;
    void transitionInTimer() ;
    void transitionPhase(Real lambda) ;

    void onBannerPressed(int) ;


private :
    //
    // Database
    //
    bool selectAd(Ad &ad) ;
    void initDatabase() ;

    struct DatabaseRecord {
        MString dir, ini, ani, tex ;
        int priority = 1 ;
        int count = 0 ;
    } ;
    std::vector<DatabaseRecord> database_ ;


    struct AdQueueItem {
        size_t databaseIndex ;
        bool yield ;
    } ;
    enum {  AD_QUEUE_SIZE = 100 } ;
    size_t adQueueIndex_ = AD_QUEUE_SIZE, nextQueueIndex_ ;
    std::array<AdQueueItem, AD_QUEUE_SIZE> adQueue_ ;

private :
    apl::AThread<> bannerThread_, serverThread_ ;
    std::atomic<bool> hidden_{false} ;

public :
    void bannerThreadBody() ;
    void serverThreadBody() ;
    */
} ;


}

#endif
