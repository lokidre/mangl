/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/

#include "texmake.h"


// Brute force method
bool TexMake::buildPlacementsBruteForce() {
    using namespace apl;

    // Sort all the images by height
    std::multimap<Index, Index, std::greater<>> texturesByHeight;

    Index index{};
    for (auto& tex: textures_) {
        if (tex.phonyImage) {
            ++index;
            continue;
        }
        texturesByHeight.insert({tex.sourceRect.h, index});
        ++index;
    }

    // continue while there are textures left
    PointI current{};

    for (auto& it: texturesByHeight) {
        auto& tex = textures_[it.second];
        if (tex.placed)
            continue;
        if (!placeImageSourceBruteForce(tex, &current))
            return false;
    }

    // Place the fonts after to fill small gaps
    for (auto& font: fonts_) {
        for (auto& glyph: font.glyphs) {
            if (glyph.phonyImage)
                continue;
            if (glyph.placed)
                continue;
            if (!placeImageSourceBruteForce(glyph, &current))
                return false;
        }
    }

    return true;

}

bool TexMake::placeImageSourceBruteForce(ImageSource& src, PointI* current) {
    using apl::verify;

    bool placeFound = false;
    auto outSize = outputImage_.getSize();

    for (int row = 0; row < outSize.h - src.targetRect.h && !placeFound; ++row) {
        for (int col = 0; col < outSize.w - src.targetRect.w && !placeFound; ++col) {
            bool foundTex = false;

            for (int j = 0; j < src.targetRect.h && !foundTex; ++j) {
                auto m = imageMask_.get() + (row + j) * outSize.w + col;

                for (int i = 0; i < src.targetRect.w && !foundTex; ++i, ++m) {
                    if (*m)
                        foundTex = true;
                }
            }

            if (!foundTex) {
                *current = {col, row};
                placeFound = true;
            }
        }
    }

    if (!placeFound)
        return false;

    //verify(placeFound, "Not enough room for: %s", src.filename);


    src.targetRect.move(*current);

    // // check if we don't have any more room
    // verify(src.targetRect.top() <= outSize.h, "Not enough room for: %s (%g,%g)", src.filename, src.targetRect.top(), outSize.h);
    // verify(src.targetRect.right() <= outSize.w, "Not enough room for: %s (%g,%g)", src.filename, src.targetRect.right(), outSize.w);

    markImagePlaced(src);

    current->x += src.targetRect.w;

    return true;
}
