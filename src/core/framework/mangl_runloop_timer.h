/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"
#include "../mangl_debug.h"

#include <functional>

namespace mangl {

struct RunLoopTimer: public apl::Singleton<RunLoopTimer> {
    friend class Framework;

    using Id = CommonId;

    using FireFunction = std::function<void(Timestamp)>;
    using FireFunctionVoid = std::function<void()>;

    static Id fire(Timestamp when, FireFunction f) {
        return instance()->scheduleItem({.f = f, .once = true, .period = when});
    }

    static Id fire(Timestamp when, FireFunctionVoid f) {
        return instance()->scheduleItem({.fv = f, .once = true, .period = when});
    }

    static Id schedule(Timestamp period, FireFunction f) {
        return instance()->scheduleItem({.f = f, .once = false, .period = period});
    }

    static Id schedule(Timestamp period, FireFunctionVoid f) {
        return instance()->scheduleItem({.fv = f, .once = false, .period = period});
    }

    static void cancel(Id id){ MDebugTrap(); }
    static void pause(Id id){ MDebugTrap(); }
    static void resume(Id id){ MDebugTrap(); }
    [[nodiscard]] bool empty() const noexcept { return items_.empty(); }
    void clear();


private:
    struct TimerItem {
        Id id{};

        FireFunction f{};
        FireFunctionVoid fv{};
        bool once{};
        Timestamp period{};


        Timestamp fireTimestamp{};

        bool cancelled{};
        bool paused{};
    };

    Id scheduleItem(TimerItem item);


    std::vector<TimerItem> items_;
    std::vector<TimerItem> queuedItems_;

    bool entered_{false};


    void timer(Timestamp timestamp);

    void maintenance();
    int currentId_ = 1;
};

}  // namespace mangl
