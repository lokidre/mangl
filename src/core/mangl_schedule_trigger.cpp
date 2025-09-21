/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_schedule_trigger.h"

#include "mangl_framework.h"

namespace mangl {

bool ScheduleTriggerTimed::triggeredCore() noexcept {
    if (active_ && Framework::currentTimestamp() >= triggerTimestamp_) {
        active_ = false;
        return true;
    }
    return false;
}

void ScheduleTriggerTimed::trigger(Timestamp when) noexcept {
    active_ = true;
    triggerTimestamp_ = Framework::currentTimestamp() + when;
}

} // namespace mangl
