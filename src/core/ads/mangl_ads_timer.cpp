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

//#include "../animation/mangl_animation.h"
//#include "../render/mangl_render.h"
//#include "../scene/mangl_scene.h"
//#include "../widgets/mangl_button.h"


namespace mangl {

void EmbedAdsNetworkClient::onTimer(Timestamp timestamp) {
    //using namespace apl;

    //if (!started_)
    //    return;

    //timestamp_ = timestamp;


    //if (scheduleAdTransition_) {
    //    scheduleAdTransition_ = false;
    //    startTransition();
    //}

    //if (scheduleAdRemove_) {
    //    scheduleAdRemove_ = false;
    //    ctx_.animation->clear();
    //    ctx_.scene->clear();
    //}

    //if (transitionOutActive_) {
    //    transitionOutTimer();
    //}

    //if (transitionInActive_) {
    //    transitionInTimer();
    //}

    //ctx_.animation->onTimer(timestamp);
}

}

#endif
