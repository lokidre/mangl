/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "app.h"
#include "screen.h"

static Main<App> manglMain_;

using namespace mangl;

void App::onInit()
{
    setupLayout();

//    Env::interfaceIdiom = sampleInterfaceIdiom_;
//    Env::setApplicationSize(sampleApplicationSize_);
//    Env::setLayoutWidth(320);

    // env->interfaceIdiom = MInterfaceIdiom::Phone ;  // Phone

    // env->setApplicationFrame({ -1, -1, 320, 480 }) ;  // old iPhone, iPhone on iPad simulation
    // env->setApplicationFrame({ -1, -1, 320*2, 480*2 }) ;  // iPhone app on iPad simulation
    // env->setApplicationFrame({ -1, -1, 320, 568 }) ;  // iPhone 7
    // env->setApplicationFrame({ -1, -1, 320*2, 568*2 }) ;  // Zoomed iPhone 7


    // env->setApplicationFrame({ -1, -1, 320, 646 }, { 44, 0, 34, 0 }) ;  // iPhone X imitation
    // env->setApplicationFrame({ -1, -1, 320, 692 }, { 44, 0, 34, 0 }) ;  // iPhone XS
    // env->setApplicationFrame({ -1, -1, 414, 896}, { 44, 0, 34, 0 }) ;  // iPhone XS Max

    // env->interfaceIdiom = M::INTERFACE_IDIOM_PAD ;  // Pad
    // env->interfaceIdiom = M::INTERFACE_IDIOM_DESKTOP ;  // Desktop
    // env->setApplicationFrame({ -1, -1, 384, 512 }) ;  // iPad
}

void App::onStart() {

    switchScreen(new Screen());
}

void App::setupLayout() {
    Size screenSize{320, 480};
    //Size screenSize{414, 896};

    Env::applicationRect = {};
    Env::interfaceIdiom = InterfaceIdiom::Phone;
    Env::setApplicationSize(screenSize);

    Env::setLayout(screenSize);
}
