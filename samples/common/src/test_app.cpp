/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "test_app.h"
#include "test_static.h"

#include "common_res.hpp"

#include <mangl/resources.h>

#ifndef MANGL_CONSOLE

namespace sample {

static Main<App> manglMain_;

App::App() {
    loadSfx();
    loadMusic();

    setup({
        .defaultButtonSound = (int)SoundsRes::Button,
        .settingsVolumeSound = (int)SoundsRes::Level,
    });

    GameMusic::setupAuto<MusicRes>();
}

void App::onInit()
{
#if APEAL_OS_DESKTOP
    Env::setInterfaceIdiom(sampleInterfaceIdiom_);
    Env::setApplicationSize(sampleApplicationSize_, sampleScreenPadding_);

    //Env::setApplicationSize(sampleApplicationSize_ * Env::pixelDensity);
    //Env::setApplicationSize(sampleApplicationSize_);
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
#endif

    texCommon.load(getCommonTexBundlePath());


    texCommon.guiFont.setDefault();

    initStyles();
}


}  // namespace sample

#endif

