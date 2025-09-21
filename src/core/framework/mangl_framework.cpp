/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_framework.h"

#include "mangl_runloop_timer.h"

#include "../mangl_styles.h"

#include "../audio/mangl_audio_system.h"

#include "../ads/mangl_ads_client.h"



namespace mangl {


Framework::Framework() {
#ifndef MANGL_CONSOLE
    render_ = new SceneRenderer;
    scene_ = render_->newScene();
#endif

    frameworkActionQueueIndex_ = 0;
}

Framework::~Framework() {
#ifndef MANGL_CONSOLE
    delete render_;
    render_ = nullptr;
#endif
}

void Framework::init() {
    Env::instance();

#ifndef MANGL_CONSOLE
    AudioSystem::initialize();
#endif

    //
    // Initialize layout
    //
#if APEAL_OS_DESKTOP
    Cord zoom{1};

#if _DEBUG

#if MANGL_DEBUG_IMITATE_TABLET
    constexpr Size DefaultPadResolution{768, 1024};
    Env::setApplicationSize(DefaultPadResolution * zoom); 
#elif MANGL_DEBUG_IMITATE_PHONE
    constexpr Size Resolution_iPhoneXSMax{414, 896};
    constexpr Padding Padding_iPhoneX{.t = 44, .b = 34};

    Env::setApplicationSize(Resolution_iPhoneXSMax * zoom, Padding_iPhoneX * zoom);
#else
    constexpr Size Resolution_iPhoneXSMax{414, 896};
    constexpr Padding Padding_iPhoneX{.t = 44, .b = 34};

    Env::setApplicationSize(Resolution_iPhoneXSMax * zoom, Padding_iPhoneX * zoom);
#endif

#else  // Not debug
    constexpr Size DefaultPadResolution{768, 1024};
    Env::setApplicationSize(DefaultPadResolution * zoom); 
#endif
#endif // Desktop

    onInit();
    onLayout();

#ifndef MANGL_CONSOLE
    //onViewport();  //TODO: check for all OSes

    render_->onInit();

    // Load styles
    M::loadDefaultStyles();
#endif


}



void Framework::onInitialized()
{
//#if MANGL_ADS && MANGL_ADS_EMBED
//    if (Env::adsBannerEnabled()) {
//        ads_->start();
//    }
//#endif
}

void Framework::onLayout() {
}

void Framework::setDefaultButtonSoundBody(SfxId resId) {
    mangl_internal::defaultButtonSound_ = resId;
}


void Framework::onViewport() {
#ifndef MANGL_CONSOLE
    render_->updateLayout();
#endif
}

void Framework::onRelease() {
#ifndef MANGL_CONSOLE
    render_->onRelease();
#endif
}

void Framework::onStart(){}

void Framework::onPause() {
    if (currentScreen_)
        currentScreen_->onPause();
}

void Framework::onResume() {
    if (currentScreen_)
        currentScreen_->onResume();
}

void Framework::onResignActive() {
    if (currentScreen_)
        currentScreen_->onResignActive();
}

void Framework::onResumeActive() {
    if (currentScreen_)
        currentScreen_->onResumeActive();
}







//
// Screens
//
void Framework::onTerminate()
{
    // Stop the renderer
    if (currentScreen_)
        currentScreen_->onTerminate();

}


void Framework::resumeScreen(ScreenBaseP screen)
{
    currentScreen_ = screen;
    currentScreen_->onResume();
}


void Framework::processBack() {
    if (currentScreen_) {
        currentScreen_->frameworkBackProcessed_ = true;
        currentScreen_->onBack();
        if (!currentScreen_->frameworkBackProcessed_ && !screensBackStack_.empty()) {
            popScreen();
        }
    }
}


}  // namespace mangl
