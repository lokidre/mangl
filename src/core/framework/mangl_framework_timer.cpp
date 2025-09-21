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

//
// Timer processor
//
struct TimerProcData {
    TimerHandler::TimerFunc func{};
    bool postfix{};
    CommonId id{};
    bool pendingErase{};
};


using TimerProcContainer = std::vector<TimerProcData>;
static TimerProcContainer timerProcs_, queuedTimerProcs_;

static CommonId currentTimerId_ = 1;

CommonId TimerHandler::registerHandler(TimerFunc&& func) noexcept {
    MDebugAssert(!isRegistered());
    apl::verify(!isRegistered(), "Timer handler already registered");
//    unregister();

    auto& item = queuedTimerProcs_.emplace_back();

    item.id = currentTimerId_++;
    item.func = std::move(func);
    item.pendingErase = false;

    timerId = item.id;

    return item.id;
}

CommonId TimerHandler::unregister() noexcept {
    if (!timerId) return timerId;


    for (auto& p: timerProcs_) {
        if (p.id == timerId) {
            p.pendingErase = true;
            break;
        }
    }

    timerId = 0;
    return timerId;
}


//
// Timer 
//
void Framework::timerPrefix()
{
    if (lastTimestamp_ == 0)
        lastTimestamp_ = currentTimestamp_;

    if (showScreenQueued_) {
        if (currentScreen_) {
            currentScreen_->onPause();

            screensBackStack_.emplace([this, parentScreen = currentScreen_] { 
                queuedResumeScreen_ = parentScreen;
            });
            switchScreen(queuedShowScreen_);
        }

        currentScreen_ = nullptr;
    }

    if (queuedResumeScreen_) {
        if (currentScreen_) {
            timerReleaseScreen(currentScreen_);
            currentScreen_ = nullptr;
        }

        resumeScreen(queuedResumeScreen_);

        queuedResumeScreen_ = nullptr;
    }


    if (queuedScreen_) {
        timerSwitchScreen(queuedScreen_);
        queuedScreen_ = nullptr;
    }

    // Manage timer processors
    if (!queuedTimerProcs_.empty()) {
        timerProcs_.insert(timerProcs_.end(), queuedTimerProcs_.begin(), queuedTimerProcs_.end());
        queuedTimerProcs_.clear();
    }

    for (auto& it: timerProcs_) {
        if (!it.pendingErase && !it.postfix)
            it.func(currentTimestamp_);
    }


    if (currentScreen_) {
        currentScreen_->onTimerPrefix(currentTimestamp_);
        currentScreen_->onTimer(currentTimestamp_);
        currentScreen_->onTimerPostfix(currentTimestamp_);
        onScreenTimer(currentTimestamp_);

        if (inScreenTransitionContext_.active) {
            inScreenTransitionContext_.processTimer(currentTimestamp_);
        }
        if (outScreenTransitionContext_.active) {
            outScreenTransitionContext_.processTimer(currentTimestamp_);
            if (!outScreenTransitionContext_.active) {
                queuedScreen_ = queuedTransitionScreen_;
            }
        }
    }

    if (timerServiceQueued_)
        timerService_->timer(currentTimestamp_);


    // Now process all registered timer processors
    bool pendingErase{};
    for (auto& it: timerProcs_) {
        if (it.pendingErase) {
            pendingErase = true;
            continue;
        }
        if (it.postfix)
            it.func(currentTimestamp_);
    }

    if (pendingErase)
        std::erase_if(timerProcs_, [](auto& it){ return it.pendingErase; });

}



void Framework::timerPostfix() {
}

}  // namespace mangl

