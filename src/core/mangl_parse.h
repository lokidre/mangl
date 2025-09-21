/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_color.h"
#include <apeal/parse/yaml.hpp>

namespace mangl {

Color parseColor(StrView text);

template <class T>
void parseNode(const apl::YamlNode& node, apl::Point2D<T>* pt) {
    using namespace apl;
    verify(node.isCollection(), "parseYamlPoint: node is not collection");

    verify(node.nodes.size() >= 2, "parseYamlPoint: node array is small");

    node.nodes[0].get(&pt->x);
    node.nodes[1].get(&pt->y);
}

template <class T>
void parseNode(const apl::YamlNode& node, apl::Size<T>* sz) {
    using namespace apl;
    verify(node.isCollection(), "parseYamlSize: node is not collection");
    verify(node.nodes.size() >= 2, "parseYamlSize: node array is small");

    node.nodes[0].get(&sz->w);
    node.nodes[1].get(&sz->h);
}

template <class T>
void parseNode(const apl::YamlNode& node, apl::ValRange<T>* rng) {
    using namespace  apl;
    verify(node.isCollection(), "parseYamlRange: node is not collection");
    verify(node.nodes.size() >= 2, "parseYamlRange: node array is small");

    node.nodes[0].get(&rng->mn);
    node.nodes[1].get(&rng->mx);
}

inline Flags parseAlign(StrView s) {
    using namespace apl;

    switch (pshHash(s)) {
    case "left"_psh: return M::AlignLeft;
    case "center"_psh: return M::AlignCenter;
    case "right"_psh: return M::AlignRight;
    default: throwError("parseAlign invalid value: %s", s);
    }
}

}  // namespace mangl
