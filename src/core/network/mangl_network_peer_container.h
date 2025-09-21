/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_common.h"
#include "mangl_network_peer.h"

#include <apeal/mutex.hpp>

#include <unordered_set>
//#include <unordered_multiset>

namespace mangl::net {

template <class PeerT>
class PeerContainer {
public:
    using Item = PeerT;
    using ItemP = Item*;
    using ItemCP = const Item*;

private:
    std::vector<ItemP> vector_;
    std::unordered_map<PeerKey, ItemP> bykey_;

    std::unordered_set<ItemP> allocated_;

public:
    ~PeerContainer() {
        clear();
    }


    auto insert(ItemP peer) noexcept {
        //Lock lock{mutex};

        vector_.push_back(peer);
        bykey_.insert({peer->key, peer});

        return peer;
    }

    auto create(PeerKey pk, DiscoveryInfoA di) noexcept {
        auto peer = new PeerT();

        peer->key = pk;
        peer->di = di;

        //allocated_.insert({peer->key, peer});
        allocated_.insert(peer);

        return insert(peer);
    }


    void erase(ItemP peer) noexcept {
        if (!peer) return;

        bykey_.erase(peer->key);
        std::erase(vector_, peer);

        if (auto allocIt = allocated_.find(peer); allocIt != allocated_.end()) {
            allocated_.erase(allocIt);
            delete peer;
        }
    }

    void remove(ItemP peer) noexcept { return erase(peer); }

    void removeDisconnected() noexcept {
        for (;;) {
            ItemP disconnected{};
            for (auto p: vector_) {
                if (PeerState::Disconnected == p->state) {
                    disconnected = p;
                    break;
                }
            }
            if (!disconnected) break;
            remove(disconnected);

            //auto disconnected = std::ranges::find_if(vector_, [](auto p) { return PeerState::Disconnected == p->state; });
            //if (disconnected == vector_.end()) break;
            //remove(*disconnected);
        }
    }


    void clear() noexcept {
        //Lock lock{mutex};
        for (auto it: allocated_) {
            delete it;
        }
        allocated_.clear();
        //for (auto peer: vector_) {
        //    delete peer;
        //}
        vector_.clear();
        bykey_.clear();
    }

    //void remove(PeerKey key) {
    //    auto it = container_.find(key);
    //    if (it == container_.end())
    //        return;
    //    auto peer = it->second;
    //}

    ItemP get(PeerKey pk) noexcept {
        //Lock lock{mutex};

        auto it = bykey_.find(pk);
        if (it == bykey_.end()) return nullptr;
        return it->second;
    }

    auto empty() const noexcept {  return vector_.empty();  }
    auto size() noexcept {  return vector_.size();  }
    auto ssize() noexcept { return std::ssize(vector_);  }

    auto begin() noexcept {  return vector_.begin(); }
    auto end() noexcept {  return vector_.end(); }

    auto begin() const noexcept {  return vector_.cbegin(); }
    auto end() const noexcept {  return vector_.cend(); }

    auto cbegin() const noexcept {  return vector_.cbegin(); }
    auto cend() const noexcept {  return vector_.cend(); }

    // Not thread safe
//    auto& data() noexcept { return vector_; }
//    const auto& data() const noexcept { return vector_; }

};

}  // namespace

