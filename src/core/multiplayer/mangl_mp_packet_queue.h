/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_types.h"

#include <apeal/buffer/ring_buffer.hpp>

namespace mangl::mmp {

enum class MpPacketAction {
    Send, 
    Disconnect,
};

class MpPacketQueue {
    struct QueueItem {
        MpPacketAction action{MpPacketAction::Send};
        MpPeerP peer{};
        std::vector<uint8_t> data;
    };

    apl::LinkedRingBuffer<QueueItem> queue_;
    //apl::RingBuffer<QueueItem> queue_;


public:
    void push(MpPeerP peer, DataParamA data, MpPacketAction action = MpPacketAction::Send) {
        auto& item = queue_.emplace();
        item.action = action;
        item.peer = peer;
        item.data.resize(data.size());
        memcpy(item.data.data(), data.data(), data.size());
    }

    auto& top() { return queue_.top(); }
    auto pop() { return queue_.pop(); }

    bool empty() const noexcept { return queue_.empty(); }
};


}  // namespace mmp


