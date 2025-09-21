/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_runloop_timer.h"
#include "../mangl_util.h"

#include "../mangl_framework.h"

namespace mangl {


void RunLoopTimer::timer(Timestamp timestamp)
{
    entered_ = true;

    for (auto &item: items_) {
        if (!item.paused && timestamp >= item.fireTimestamp) {
            if (item.f)
                item.f(timestamp);
            if (item.fv)
                item.fv();
            if (item.once)
                item.cancelled = true;
            else
                item.fireTimestamp = item.fireTimestamp + item.period;
        }
    }

    maintenance();

    entered_ = false;
}

void RunLoopTimer::maintenance()
{
    for (auto it = items_.begin(), end = items_.end(); it != end; ) {
        if (it->cancelled) {
            it = items_.erase(it);
            end = items_.end();
            continue;
        }
        ++it;
    }

    if (items_.empty()) {
        Framework::timerServiceQueued_ = false;
    }
}

auto RunLoopTimer::scheduleItem(TimerItem item) -> Id
{
    auto fw = Framework::instance();

    Framework::timerServiceQueued_ = true;
    Framework::timerService_ = this;

    item.fireTimestamp = Framework::timestamp() + item.period;
    item.id = M::generateId<Id>(); //currentId_++;

    if (entered_)
        queuedItems_.push_back(item);
    else
        items_.push_back(item);

    return item.id;
}


void RunLoopTimer::clear()
{
    for (auto &item: items_) {
        item.cancelled = true;
    }

    if (!entered_)
        maintenance();
}

}  // namespace mangl
