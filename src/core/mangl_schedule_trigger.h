/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_types.h"

namespace mangl {

struct ScheduleTrigger {

    ScheduleTrigger() noexcept = default;
    explicit ScheduleTrigger(bool a) noexcept: active_{a} {}

    void trigger() noexcept { active_ = true; }
    bool operator=(bool a) noexcept { active_ = a;  return a; }  // for chain assignment

    bool triggered() noexcept { return operator bool(); }

    [[nodiscard]] bool active() const noexcept { return active_; }
    void set() noexcept { active_ = true; }
    void clear() noexcept { active_ = false; }

    bool operator()() noexcept {
        if (active_) {
            active_ = false;
            return true;
        }
        return false;
    }

    explicit operator bool() noexcept {
        if (active_) {
            active_ = false;
            return true;
        }
        return false;
    }


private:
    bool active_{};
};

struct ScheduleTriggerTimed {

    ScheduleTriggerTimed() noexcept = default;
    explicit ScheduleTriggerTimed(Timestamp when) noexcept { trigger(when); }


    void trigger(Timestamp when) noexcept;
    void operator=(Timestamp when) noexcept { trigger(when); }

    bool active() const noexcept { return active_; }
    void clear() noexcept { active_ = false; }

    bool triggered() noexcept { return active_ && triggeredCore(); }

    bool operator()() noexcept {  return active_ && triggeredCore(); }
    explicit operator bool() noexcept { return active_ && triggeredCore(); }

private:
    Timestamp triggerTimestamp_{};
    bool active_{};
    bool triggeredCore() noexcept;
};


}  // namespace mangl
