/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "main_data.h"

namespace toolkit {

using RF = ResolutionFamily;

using enum ResolutionFamily;

//std::vector<std::pair<ResolutionFamily, String>> resolutionFamilies_{
std::map<ResolutionFamily, String> resolutionFamilies_{
    {RF::Phone, "Phone"},
    {RF::Pad, "Pad"},
    {RF::Desktop, "Desktop"},
    {RF::Square, "Square"},
    {RF::ThreeTwo, "3/2"},
    {RF::Banner, "Banner"},
};

std::vector<ResolutionInfo> resolutionInfos_{
    {RF::Phone, "320x480", {320, 480}, 1 },
    {RF::Phone, "320x568", {320, 568}, 1 },
    {RF::Phone, "320x646 (unibrow)", {320, 646}, 1, { 44, 0, 34, 0 } },
    {RF::Phone, "375x812 (i X)", {375, 812}, 1, { 44, 0, 34, 0 } },
    {RF::Phone, "414x896 (i XS Max)", {414, 896}, 1, { 44, 0, 34, 0 } },
    {RF::Phone, "640x960", {640, 960}, 2 },
    {RF::Phone, "640x1136", {640, 1136}, 2 },


    {RF::Pad, "384x512", {384, 512}, 1},
    {RF::Pad, "768x1024", {768, 1024}, 2},
    {RF::Pad, "1536x2048", {1536, 2048}, 4},

    {RF::Desktop, "480x640", {480, 640}, 1},
    {RF::Desktop, "600x800", {600, 800}, 2},
    {RF::Desktop, "768x1024d", {768, 1024}, 2},

    {RF::ThreeTwo, "60x40", {60, 40}, 1 },
    {RF::ThreeTwo, "120x80", {120, 80}, 1 },
    {RF::ThreeTwo, "180x120", {180, 120}, 1 },

    {RF::Square, "32x32", {32, 32}, 1 },
    {RF::Square, "40x40", {40, 40}, 1 },
    {RF::Square, "64x64", {64, 64}, 1 },
    {RF::Square, "100x100", {100, 100}, 1 },
    {RF::Square, "200x200", {200, 200}, 1 },
    {RF::Square, "400x400", {400, 400}, 1 },
    {RF::Square, "600x600", {600, 600}, 1 },

    {RF::Banner, "320x50", {320, 50}, 1 },
    {RF::Banner, "384x50", {384, 50}, 1 },
    {RF::Banner, "640x100", {640, 100}, 2 },
    {RF::Banner, "768x100", {768, 100}, 2 },
};



std::vector<int> layoutZooms_{25, 50, 75, 100, 125, 150, 200, 400, 600};

std::vector<ProjectFileCollectionInfo> projectCollections_{
    {"Project"}, {"Screen"}, {"Animation"}, {"Background"}, {"Model"},
};

}  // namespace toolkit
