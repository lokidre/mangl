/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#include "mangl_blueprint_loader.h"

#include "../mangl_parse.h"

#include <apeal/parse/vector.hpp>

namespace mangl::blueprint {

using namespace apl;
using namespace apl::literals;


BlueprintColor Loader::parseLayoutColor(YamlNodeA yaml, NodeP node)
{
    BlueprintColor c;

    if (!yaml.value.empty()) {
        c.value = parseColor(yaml.value);
    }

    return c;
}

int Loader::parseLayoutColors(YamlNodeA yaml, NodeP node, ColorsSet& colors)
{
    auto components = parseStdVectorTrim<StrView>(yaml.value, ',');
    auto maxComps = std::ssize(colors);
    verifySource(std::ssize(components) <= maxComps, yaml, "More than 4 colors supplied");

    int count = 0;
  
    for (auto& c: components) {
        colors[count].value = parseColor(c);
        ++count;
    }  

    return count;
}

}  // namespace mangl::blueprint
