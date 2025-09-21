/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_mp_match.h"

namespace mangl::mmp {

class MpMatchContainer {

    using Key = MpMatchKey;
    using Id = String;
    using IdA = const String&;

    using Item = MpMatch;
    using ItemP = Item*;
    using ItemCP = const Item*;

    std::vector<ItemP> vector_;
    std::unordered_map<Key, ItemP> bykey_;
    std::unordered_map<Id, ItemP> byid_;


public:
    ~MpMatchContainer() {
        clear();
    }

    void clear() noexcept {
        for (auto m: vector_)
            delete m;
        bykey_.clear();
        byid_.clear();
        vector_.clear();
    }

    auto create(Key mk, IdA mid) {
        auto match = new MpMatch();

        match->matchKey = mk;
        match->matchId = mid;

        vector_.push_back(match);
        bykey_.insert({mk, match});
        byid_.insert({mid, match});

        return match;
    }

    void remove(ItemCP match) noexcept {
        if (!match) return;

        bykey_.erase(match->matchKey);
        byid_.erase(match->matchId);

        std::erase(vector_, match);
        delete match;
    }

    void erase(ItemCP match) noexcept {  return remove(match);  }


    ItemP get(int matchKey) noexcept {
        auto it = bykey_.find(matchKey);
        if (it == bykey_.end()) return nullptr;
        return it->second;
    }

    ItemP byId(IdA id) noexcept {
        auto it = byid_.find(id);
        if (it == byid_.end()) return nullptr;
        return it->second;
    }

    auto empty() const noexcept { return vector_.empty(); }
    auto size() const noexcept { return vector_.size(); }
    auto ssize() const noexcept { return std::ssize(vector_); }


    auto begin() noexcept {  return vector_.begin();  }
    auto end() noexcept {  return vector_.end();  }

    auto begin() const noexcept {  return vector_.begin();  }
    auto end() const noexcept {  return vector_.end();  }

    auto cbegin() const noexcept {  return vector_.cbegin();  }
    auto cend() const noexcept {  return vector_.cend();  }

};

}  // namespace mangl::mmp
