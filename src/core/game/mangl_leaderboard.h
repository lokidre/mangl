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

namespace mangl {

struct Leaderboard;
using LeaderboardP = Leaderboard*;

struct Leaderboard {

    String title;
    std::size_t maxValues{50};

    String gameCenterId; // Apple Game Center leaderboard ID

    using ScoreList = std::vector<int>;


    Leaderboard() {}
    Leaderboard(StrView i): id_{i} { key_ = makeKey(); }
    Leaderboard(StrView i, StrView t): id_{i}, title{t} { key_ = makeKey(); }

    void setId(StrView lid) {
        id_ = lid;
        key_ = makeKey();
    }


    void post(int score);

    void clear() {
        data_.clear();
        saveData();
    }

    int topScore() {
        loadData();
        return data_.empty() ? 0 : data_.front();
    }

    ScoreList& data() {
        loadData();
        return data_;
    }

    auto getValue(Index index) {
        MDebugTrapAssert(index < std::ssize(data_));
        return data_[index];
    }

    bool isHighScore(int score) {
        loadData();
        return data_.empty() ? true : score >= data_.front();
    }

    bool isTop(int score, int limit) {
        loadData();
        return count() < limit ? true : score >= data_[limit - 1];
    }

    int count() { 
        loadData();
        return apl::count(data_); 
    }

private:
    String id_, key_;
    ScoreList data_;

    String makeKey() { return "mangl_leaderboard:" + id_; }

    void loadData();
    void saveData();
    bool loaded_{false};
};

} // namespace mangl
