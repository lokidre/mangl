/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../mangl_framework.h"
#include "../main/mangl_main.h"


namespace mangl {

FrameworkP Framework::instance_{};
ThreadId Framework::mainThreadId{};

namespace mangl_internal {
int defaultButtonSound_ = InvalidSoundButton;
}  // namespace mangl_internal


bool Framework::haveAppParams_{false};
AppParams Framework::appParams_{};


Timestamp Framework::currentTimestamp_{};
Timestamp Framework::lastTimestamp_{};
std::atomic<Timestamp> Framework::currentTimestampAtomic_{};
SceneRenderer *Framework::render_{};

SceneP Framework::scene_{};

bool Framework::timerFirstRun_{true};
bool Framework::timerServiceQueued_{false};
RunLoopTimer* Framework::timerService_{};


ScreenTransitionContext Framework::inScreenTransitionContext_{};
ScreenTransitionContext Framework::outScreenTransitionContext_{};
ScreenBaseP Framework::transitionQueuedScreen_{};

bool Framework::interactionEnabled_{true};

std::stack<std::function<void()>> Framework::screensBackStack_;
ScreenBaseP Framework::currentScreen_{};
ScreenBaseP Framework::queuedScreen_{};
ScreenBaseP Framework::queuedTransitionScreen_{};

ScheduleTrigger Framework::showScreenQueued_;
ScreenBaseP Framework::queuedShowScreen_{};

ScreenBaseP Framework::queuedResumeScreen_{};

MFrameworkStats Framework::debugStats{};

}  // namespace mangl


