/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>

std::vector<std::pair<PointI, std::pair<int, int>>> regionsDetails;


void TexMake::initializeDFS() {
    auto outSize = outputImage_.getSize();

    regionsDetails.push_back({{0, 0}, {outSize.w, outSize.h}});
}

bool TexMake::buildPlacementsDFS() {

    std::vector<ImageSource*> unifiedSources;

    std::size_t fontGlyphsCount = 0;

    for (auto& font: fonts_) fontGlyphsCount += font.glyphs.size();

    unifiedSources.reserve(textures_.size() + fontGlyphsCount);

    for (auto& tex: textures_) {
        unifiedSources.push_back(&tex);
    }

    for (auto& font: fonts_) {
        for (auto& glyph: font.glyphs) {
            if (!glyph.phonyImage) // Ensure we only add real glyphs
                unifiedSources.push_back(&glyph);
        }
    }

    std::ranges::sort(unifiedSources, [](const ImageSource* a, const ImageSource* b) {
        return (a->targetRect.h * a->targetRect.w) > (b->targetRect.h * b->targetRect.w);
    });

    for (auto* source: unifiedSources) {
        if (source->placed)
            continue;

        bool placed = false;

        for (const auto& region: regionsDetails) {
            PointI topLeft = region.first;
            placed = dfsGuillotine(*source, topLeft, region.second);
            if (placed)
                break;
        }

        if (!placed) {
            return false;
            //std::cout << "Failed to place source with height: " << source->targetRect.h << std::endl;
            //apl::throwError("Failed to place source with height: %s", source->targetRect.h);
        }
    }

    return true;
}


bool TexMake::dfsGuillotine(ImageSource& source, const PointI& topLeft, const std::pair<int, int>& regionSize) {
    if (canPlaceImageSource(source, topLeft)) {
        source.targetRect.move(topLeft);
        markImagePlaced(source);
        //source.targetRect
        //markPosition(source, topLeft);
        //source.placed = true;

        std::pair<PointI, std::pair<int, int>> newRegion1, newRegion2;
        splitRegion(topLeft, regionSize, source, newRegion1, newRegion2);

        if (newRegion1.second.second > 0) {
            regionsDetails.push_back(newRegion1);
        }
        if (newRegion2.second.first > 0) {
            regionsDetails.push_back(newRegion2);
        }

        return true;
    }
    return false;
}



bool TexMake::canPlaceImageSource(const ImageSource& source, const PointI& pos) {
    auto outSize = outputImage_.getSize();


    if (pos.x < 0 || pos.y < 0 || pos.x + source.targetRect.w > outSize.w || pos.y + source.targetRect.h > outSize.h) {
        //std::cout << "Image source out of bounds" << std::endl;
        return false;
    }

    // Check for overlaps in the mask
    for (int j = 0; j < source.targetRect.h; ++j) {
        auto m = imageMask_.get() + (pos.y + j) * outSize.w + pos.x;
        for (int i = 0; i < source.targetRect.w; ++i) {
            if (*m++) {
                //std::cout << "Overlap detected for image source" << std::endl;
                return false;
            }
        }
    }

    return true;
}

/*
void TexMake::markPosition(const ImageSource& source, const PointI& pos) {
    auto outSize = outputImage_.getSize();
    // std::cout << "Marking position for image source at: (" << pos.x << ", " << pos.y << ") with size: ("
    //           << source.targetRect.w + 2 << ", " << source.targetRect.h + 2 << ")" << std::endl;

    for (int y = 0; y < source.targetRect.h; ++y) {
        auto m = imageMask_.get() + (pos.y + y) * outSize.w + pos.x;
        for (int x = 0; x < source.targetRect.w; ++x) {
            *m++ = 1;
        }
    }
}
*/

void TexMake::splitRegion(const PointI& topLeft, const std::pair<int, int>& regionSize, const ImageSource& source,
                          std::pair<PointI, std::pair<int, int>>& newRegion1,
                          std::pair<PointI, std::pair<int, int>>& newRegion2) {
    int remainingHeight = regionSize.second - (source.targetRect.h);
    int remainingWidth = regionSize.first - (source.targetRect.w);

    newRegion1 = {{topLeft.x, topLeft.y + source.targetRect.h}, {regionSize.first, std::max(remainingHeight, 0)}};
    newRegion2 = {{topLeft.x + source.targetRect.w, topLeft.y}, {std::max(remainingWidth, 0), regionSize.second}};

    // std::cout << "Region split into two: Region1 (" << newRegion1.first.x << ", " << newRegion1.first.y << ") - Size ("
    //           << newRegion1.second.first << ", " << newRegion1.second.second << ")" << std::endl;
    // std::cout << "Region2 (" << newRegion2.first.x << ", " << newRegion2.first.y << ") - Size ("
    //           << newRegion2.second.first << ", " << newRegion2.second.second << ")" << std::endl;
}


