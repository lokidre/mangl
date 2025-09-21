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
#include <set>
#include <map>
#include <unordered_map>
#include <array>

enum class SampleCharEnum: char {
	Invalid = 0,
	ValueA = 'A',
	ValueB = 'B',
};

enum class SampleIntEnum: int {
	Invalid = 0,
	Value1 = 123,
	Value2 = 3435,
};


struct FlatObject {
    char charVal;
    unsigned char ucharVal;

    int intVal;
    unsigned int uintVal;

    long longVal;
    unsigned long ulongVal;

    long shortVal;
    unsigned long ushortVal;

    float floatVal;
    double doubleVal;

    std::string stringVal;

    SampleCharEnum charEnum;
    SampleIntEnum intEnum;
    
    std::vector<std::string> vectorVal;
    std::array<float, 10> arrayVal;

    std::map<std::string, double> mapVal;
    std::unordered_map<std::string, std::string> unorderedMapVal;

    std::set<int> setVal;

    auto operator<=>(const FlatObject&) const noexcept = default;
};
