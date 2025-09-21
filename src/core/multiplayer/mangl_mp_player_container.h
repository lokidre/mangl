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
#include "mangl_mp_player.h"

#include <apeal/noncopyable.hpp>

namespace mangl::mmp {

class MpPlayerContainer: public apl::NonCopyable {

    using Key = MpPlayerKey;
    using Id = String;

    using Item = MpPlayer;
    using ItemP = Item*;
    using ItemCP = const Item*;


    std::vector<ItemP> vector_;
    std::unordered_map<Key, ItemP> bykey_;
    std::unordered_map<Id, ItemP> byid_;

    std::unordered_map<PeerKey, ItemP> bypeer_;


public:
	~MpPlayerContainer() {
	    clear();
	}

	void clear() noexcept {
	    for (auto p: vector_) {
	        delete p;
	    }
	    bykey_.clear();
	    byid_.clear();
	    vector_.clear();

	    bypeer_.clear();
	}

    // TODO: initialization, cleanup
    auto create(MpPeerP peer, Key pk, const Id& pid) noexcept {
        auto player = new Item();

        player->peer = peer;
        player->peerKey = peer->key;
        player->key = pk;
        player->id = pid;

        peer->player = player;

        vector_.push_back(player);

        bykey_.insert({pk, player});
        byid_.insert({pid, player});

        bypeer_.insert({player->peerKey, player});

        return player;
    }


    auto updatPeer(ItemP player, MpPeerP peer) noexcept {
        bypeer_.erase(player->peerKey);
        player->peer = peer;
        player->peerKey = peer->key;
        bypeer_.insert({player->peerKey, player});
    }



    ItemP byPeer(PeerKey pk) noexcept {
        auto it = bypeer_.find(pk);
        if (it == bypeer_.end()) return nullptr;
        return it->second;
    }

    auto byPeer(MpPeerCP peer) noexcept { return byPeer(peer->key); }


    ItemP get(Key pk) noexcept {
        auto it = bykey_.find(pk);
        if (it == bykey_.end()) return nullptr;
        return it->second;
    }



    void remove(ItemCP player) noexcept {
        bypeer_.erase(player->peerKey);

        bykey_.erase(player->key);
        byid_.erase(player->id);

        std::erase(vector_, player);
        delete player;
    }

    void erase(ItemCP player) noexcept {  return remove(player); }



    auto empty() const noexcept { return vector_.empty(); }
    auto size() const noexcept { return vector_.size(); }
    auto ssize() const noexcept { return std::ssize(vector_); }

//    auto& data() noexcept { return vector_; }
//    const auto& data() const noexcept { return vector_; }


    auto begin() noexcept { return vector_.begin(); }
    auto end() noexcept { return vector_.end(); }

    auto begin() const noexcept { return vector_.begin(); }
    auto end() const noexcept { return vector_.end(); }

    auto cbegin() const noexcept { return vector_.cbegin(); }
    auto cend() const noexcept { return vector_.cend(); }



};

}  // namespace

