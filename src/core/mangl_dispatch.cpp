/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_dispatch.h"

namespace mangl {

#if APEAL_OS_MAC

#else


std::atomic_bool Dispatch::queueEmpty_{};  // Use atomic bool for speed
Dispatch::Mutex Dispatch::queueMutex_{};
apl::LinkedRingBuffer<DispatchQueueItem> Dispatch::queue_{};


void Dispatch::asyncMain(DispatchBlock block) {

    scheduleDispatchItem({.block = std::move(block)});
}

void Dispatch::asyncMainDelayed(DispatchBlock block, Timestamp when) {
    scheduleDispatchItem({.block = std::move(block), .when = when});
}


void Dispatch::scheduleDispatchItem(DispatchQueueItem&& item) {

    {
        Lock lock{queueMutex_};

        //if (queue_.full()) {
        //    //MDebugFunc("Queue is full");
        //    queue_.reserve(queue_.max_size() * 2);
        //}

       queue_.emplace(std::move(item));

        // if (queue_.beginIndex() != 0) {
        //     int a = 0;
        // }

        //queue_.push(item);

        //auto& head = queue_.emplace_head();
        //head = item;

    }

    queueEmpty_ = false;
}


void Dispatch::process() {
    if (queueEmpty_)
        return;

    DispatchQueueItem item{};

    for(;;) {
        
        {
            Lock lock{queueMutex_};
            if (queue_.empty()) {
                queueEmpty_ = true;
                return;
            }

            item = queue_.top();
            queue_.pop();
        }

        item.block();
    }

}


#endif

} // namespace mangl
