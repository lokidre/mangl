/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_framework.h"

namespace mangl {

void Framework::switchScreen(ScreenBaseP screen) noexcept {

    // Check if there is a transition for current screen
    if (!currentScreen_ || outScreenTransitionContext_.transition == Transition::None) {
        queuedScreen_ = screen;
        return;
    }

    // If the current screen needs transition

    // Cancel the screen's in transition if active
    inScreenTransitionContext_.finish();

    // If there is a current screen start the out transition
    if (currentScreen_) {
        outScreenTransitionContext_.start(currentScreen_, currentTimestamp_);
    }

    queuedTransitionScreen_ = screen;
}



void Framework::timerSwitchScreen(ScreenBaseP newScreen) {

    if (currentScreen_) {
        timerReleaseScreen(currentScreen_);
        currentScreen_ = nullptr;
    }

    currentScreen_ = newScreen;

    if (currentScreen_) {
        currentScreen_->onStartPrefix();
        currentScreen_->onStart();
        currentScreen_->onStartPostfix();
        currentScreen_->frameworkKeyBegan_ = false;

        onScreenStart(currentTimestamp_);

        if (inScreenTransitionContext_.transition != Transition::None) {
            inScreenTransitionContext_.start(currentScreen_, currentTimestamp_);
        }
    }
}

void Framework::timerReleaseScreen(ScreenBaseP screen) {
    screen->onReleasePrefix();
    screen->onRelease();
    screen->onReleasePostfix();
    delete screen;
}




}  // namespace mangl

