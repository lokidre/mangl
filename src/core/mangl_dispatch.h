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

using DispatchBlock = std::function<void()>;

#if APEAL_OS_DESKTOP || APEAL_OS_ANDROID
struct DispatchQueueItem {
    DispatchBlock block;
    Timestamp when;
};
#endif

class Dispatch {
public:
    static void asyncMain(DispatchBlock block);
    static void asyncMainDelayed(DispatchBlock block, Timestamp when);


#if APEAL_OS_DESKTOP || APEAL_OS_ANDROID
public:
    // Call from the main thread
    static void process();

private:
    using Mutex = std::mutex;
    using Lock = std::scoped_lock<Mutex>;

    static std::atomic_bool queueEmpty_;  // Use atomic bool for speed
    static Mutex queueMutex_;
    static apl::LinkedRingBuffer<DispatchQueueItem> queue_;

    static void scheduleDispatchItem(DispatchQueueItem&& item);
#endif
};

}  // namespace mangl
