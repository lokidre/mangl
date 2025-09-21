/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <apeal/parse/yaml.hpp>

namespace mangl {

template <class T>
void formatNode(apl::YamlBuilder& yaml, StrView name, const apl::Point2D<T>& pt) {
    yaml.flowSeq(name, pt.x, pt.y);
}

template <class T>
void formatNode(apl::YamlBuilder& yaml, StrView name, const apl::Size<T>& sz) {
    yaml.flowSeq(name, sz.w, sz.h);
}


}  // namespace mangl


