/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_network_event.h"
#include "mangl_network_peer.h"

namespace mangl::net {

struct EventQueue {

    Mutex mutex;

    bool empty() noexcept {
        if (empty_) return true;
        ScopedLock lock{mutex};
        return events_.empty();
    }

    void clear() noexcept {
        if (empty_) return;
        ScopedLock lock{mutex};
        events_.clear();
    }

    void pushError(PeerKey pk, int code, StrView message) noexcept {
        empty_ = false;
        ScopedLock lock{mutex};
        auto& ev = push();
        ev.reset(Event::Type::Error);
        ev.peerKey = pk;
        ev.packet.code = code;
        ev.packet.feed(message);
    }

    void pushData(PeerKey pk, DataParamA data) noexcept {
        empty_ = false;
        ScopedLock lock{mutex};
        auto& ev = push();
        ev.reset(Event::Type::Packet);
        ev.peerKey = pk;
        ev.packet.feed(data);
    }

    using DataProvider = std::function<void (std::byte* data, std::size_t len)>;
    void pushData(PeerKey pk, std::size_t len, const DataProvider& provider) noexcept {
        empty_ = false;
        ScopedLock lock{mutex};
        auto& ev = push();
        ev.reset(Event::Type::Packet);
        ev.peerKey = pk;
        provider(ev.packet.alloc(len), len);
    }

    void pushState(PeerP peer) noexcept {
        pushState(peer->key, peer->state);
    }

    void pushUpdateState(PeerP peer, PeerState state) noexcept {
        peer->state = state;
        pushState(peer);
    }

    void pushState(PeerKey key, PeerState state) noexcept {
        empty_ = false;

        ScopedLock lock{mutex};
        auto& ev = push();
        ev.reset(Event::Type::State);
        ev.peerKey = key;
        ev.peerState = state;
    }

    void pushUpdateFound(PeerP peer, DataParamA data) noexcept {
        peer->state = PeerState::Found;
        pushFound(peer->key, data);
    }


    void pushFound(PeerKey key, DataParamA data) noexcept {
        empty_ = false;
        ScopedLock lock{mutex};
        auto& ev = push();
        ev.reset(Event::Type::Found);
        ev.peerKey = key;
        ev.packet.feed(data);
    }

    bool pop(Event &ev) noexcept {
        if (empty_)
            return false;

        ScopedLock lock{mutex};

        if (events_.empty()) {
            empty_ = true;
            return false;
        }

        ev = events_.top();
        events_.pop();
        empty_ = events_.empty();
        return true;
    }


private:

    Event& push() noexcept {
        if (events_.full()) {
            //MDebugFunc("Queue is full");
            events_.reserve(events_.max_size()*2);
        }

        //return events_.emplace_head();
        return events_.emplace_back();
    }

    std::atomic_bool empty_;  // Use atomic bool for speed

    //static constexpr auto MaxEvents = 40;
    //apl::RingBuffer<Event, MaxEvents> events_;
    //apl::RingBuffer<Event> events_;
    apl::LinkedRingBuffer<Event> events_;
};

}  // namespace mangl::net

