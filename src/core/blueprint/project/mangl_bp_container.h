/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/noncopyable.hpp>

#include <vector>
#include <unordered_map>

namespace mangl::bp {

template <class T>
struct ContainerById: public apl::NonCopyable {

    std::vector<T*> items;
    std::unordered_map<String, T*> byId;

    ~ContainerById() noexcept {
        clear();
    }

    void clear() noexcept {

        for (auto& item: items) {
            delete item;
        }

        items.clear();
        byId.clear();
    }

    T* insert(const String& id) {
        auto item = new T();
        item->id = id;
        items.push_back(item);
        byId.insert({id, item});
        return item;
    }
};

    
} // namespace mangl::bp 
