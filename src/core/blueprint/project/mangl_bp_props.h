/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_bp_coord.h"

#include <apeal/noncopyable.hpp>

#include <unordered_map>
#include <vector>

namespace mangl::bp {

enum class PropType: int {
    Unknown = 0,

    // Layout
    Left,
    Right,
    Top,
    Bottom,

    // Colors, opacity, visibility
    Color,
    Opacity,
    Visibility,

};

struct Prop {
    PropType type{};
    CoordUnit units{};

    String text;
    std::vector<Color> colors;
    Cord cord{};
};
using PropP = Prop*;

struct PropsContainer: apl::NonCopyable {
    std::vector<PropP> props;
    std::unordered_map<PropType, Prop*> byType;

    ~PropsContainer() noexcept {
        clear();
    }

    void clear() noexcept {
        for (auto& prop: props) {
            delete prop;
        }
    }


    PropP add(PropType type) {
        auto foundIt = byType.find(type);
        apl::verify(foundIt != byType.end(), "Duplicate prop: %d", (int)type);

        auto prop = new Prop;
        prop->type = type;
        props.push_back(prop);
        byType.insert({type, prop});
        return prop;
    }

    PropP find(PropType type) {
        auto it = byType.find(type);
        if (it == byType.end())
            return nullptr;
        return it->second;
    }

    const PropP find(PropType type) const {
        auto it = byType.find(type);
        if (it == byType.end())
            return nullptr;
        return it->second;
    }

    auto begin() { return props.begin(); }
    auto end() { return props.end(); }
    auto begin() const { return props.begin(); }
    auto end() const { return props.end(); }
    auto cbegin() const { return props.cbegin(); }
    auto cend() const { return props.cend(); }

};

}
