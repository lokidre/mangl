/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_leaderboard.h"

#include "../social/mangl_game_center.h"
#include "../mangl_storage.h"

#include <apeal/parse.hpp>

namespace mangl {


//void Leaderboard::postGameCenter(int score) {
//    GameCenter::instance()->postScore(gameCenterId, score);
//}


void Leaderboard::loadData() {
    if (loaded_)
        return;

    String sdata;

    StatsStorage::instance()->get(key_, &sdata);

    auto&& sv = apl::parseStdVector<StrView>(sdata, ',');
    if (sv.empty())
        return;

    data_.clear();
    data_.reserve(std::min(maxValues, sv.size()));

    for (auto&& s: sv) {
        if (s.empty())
            continue;
        auto v = apl::parseInt(s, 0);
        if (v == 0)
            continue;
        data_.push_back(v);
        if (data_.size() >= maxValues)
            break;
    }

    loaded_ = true;
}

void Leaderboard::saveData() { StatsStorage::instance()->setf(key_, "%z", data_); }

void Leaderboard::post(int score) {
    loadData();

    if (data_.empty() || score <= data_.back()) {
        data_.push_back(score);
    } else if (score >= data_.front()) {
        data_.insert(data_.begin(), score);
    } else {
        // We know that the score is somewhere inside
        // insert the new item
        for (auto it = data_.begin(), end = data_.end(); it != end; ++it) {
            if (score >= *it) {
                data_.insert(it, score);
                break;
            }
        }
    }

    if (data_.size() > maxValues)
        data_.erase(data_.begin() + maxValues);
    saveData();
}

} // namespace mangl
