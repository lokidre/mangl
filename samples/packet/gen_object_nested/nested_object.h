/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <string>
#include <vector>
#include <map>

struct SubSubObject {
    std::map<std::string, std::string> mapVal;
    auto operator<=>(const SubSubObject&) const noexcept = default;
};

struct SubObject {
    SubSubObject subSubVal;
    std::vector<std::string> vectorVal;

    auto operator<=>(const SubObject&) const noexcept = default;
};

struct NestedObject {
    char charVal;
    unsigned char ucharVal;

    int intVal;
    unsigned int uintVal;

    float floatVal;
    double doubleVal;

    SubObject subVal;

    auto operator<=>(const NestedObject&) const noexcept = default;
};
