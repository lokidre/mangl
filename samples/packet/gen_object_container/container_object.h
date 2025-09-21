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

namespace container_test {

struct ContainerPodValue {
    int intVal;
    double doubleVal;

    auto operator<=>(const ContainerPodValue&) const noexcept = default;
};

struct ContainerComplexValue {
    int intVal;
    double doubleVal;
    std::string stringVal;

    auto operator<=>(const ContainerComplexValue&) const noexcept = default;
};

struct ContainerVectorValue {
    std::vector<int> podVecVal;

    auto operator<=>(const ContainerVectorValue&) const noexcept = default;
};

struct ContainerVectorObjectValue {
    std::vector<ContainerComplexValue> complexVecVal;

    auto operator<=>(const ContainerVectorObjectValue&) const noexcept = default;
};

struct ContainerObject {
    char charVal{};
    unsigned char ucharVal{};

    int intVal{};
    unsigned int uintVal{};

    long longVal{};
    unsigned long ulongVal{};

    short shortVal{};
    unsigned short ushortVal{};

    float floatVal{};
    double doubleVal{};

    std::vector<ContainerPodValue> podVector;
    std::vector<ContainerVectorValue> vecVector;
    std::vector<ContainerVectorObjectValue> complexVector;

    std::array<int, 10> intArray{};
    std::array<ContainerPodValue, 6> podArray{};

    std::map<int, int> mapIntInt;
    std::map<int, ContainerPodValue> mapIntStruct;
    //std::unordered_multimap<std::string, ContainerPodValue> mapStringStruct;
    std::multimap<std::string, ContainerPodValue> mapStringStruct;



    auto operator<=>(const ContainerObject&) const noexcept = default;
};

}  // namespace container_test

