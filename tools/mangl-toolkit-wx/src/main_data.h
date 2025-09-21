/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "main_types.hpp"

enum class ResolutionFamily {
    Square,
    ThreeTwo,
    Banner,
    Phone,
    Pad,
    Desktop,
};

struct ResolutionInfo {
    ResolutionFamily family;
    String title;
    Size layout;
    int upscale;
    Padding padding;
};

struct ProjectFileCollectionInfo {
    String title;
};

//extern std::vector<std::pair<ResolutionFamily, String>> resolutionFamilies_;
extern std::map<ResolutionFamily, String> resolutionFamilies_;
extern std::vector<ResolutionInfo> resolutionInfos_;
extern std::vector<int> layoutZooms_;
extern std::vector<ProjectFileCollectionInfo> projectCollections_;
