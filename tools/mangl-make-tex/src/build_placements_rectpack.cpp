/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

#include "rectpack2D/finders_interface.h"

using namespace rectpack2D;

void TexMake::buildPlacementsRectPack2D() {
    using namespace apl;

    auto outSize = outputImage_.getSize();

    // // mask
    // imageMask_.alloc(outputImage_.size.w * outputImage_.size.h);
    // imageMask_.zero();
    // imageMask_[0] = 1; // the very first pixel (0,0) is reserved

    constexpr bool allow_flip = false;

    const auto runtime_flipping_mode = flipping_option::DISABLED;

    using spaces_type = empty_spaces<allow_flip, default_empty_spaces>;

    using rect_type = output_rect_t<spaces_type>;

    auto report_successful = [](rect_type&) { return callback_result::CONTINUE_PACKING; };

    auto report_unsuccessful = [](rect_type&) { return callback_result::ABORT_PACKING; };

    const auto max_side = 1000;

    const auto discard_step = -4;

    std::multimap<Index, Index, std::greater<>> texturesBySide;

    Index index{};
    for (auto& tex: textures_) {
        if (tex.phonyImage) {
            ++index;
            continue;
        }
        auto max = std::max(tex.sourceRect.h, tex.sourceRect.w);
        texturesBySide.insert({max, index});
        ++index;
    }

    PointI current{1, 1};
    std::vector<rect_type> rectangles;

    for (auto& it: texturesBySide) {
        auto& tex = textures_[it.second];
        auto w = tex.sourceRect.w;
        auto h = tex.sourceRect.h;
        rectangles.emplace_back(rect_xywh(0, 0, w, h));
    }

    for (auto& font: fonts_) {
        for (auto& glyph: font.glyphs) {
            if (glyph.phonyImage)
                continue;
            auto w = glyph.sourceRect.w;
            auto h = glyph.sourceRect.h;
            rectangles.emplace_back(rect_xywh(0, 0, (int)w, (int)h));
        }
    }

    auto report_result = [&rectangles](const rect_wh& result_size) {
        std::cout << "Resultant bin: " << result_size.w << " " << result_size.h << std::endl;

        for (const auto& r: rectangles) {
            std::cout << r.x << " " << r.y << " " << r.w << " " << r.h << std::endl;
        }
    };


    {
        /*

           Example 1: Find best packing with default orders.
            If you pass no comparators whatsoever,
            the standard collection of 6 orders:
            by area, by perimeter, by bigger side, by width, by height and by "pathological
           multiplier"
            - will be passed by default.
        */

        const auto result_size = find_best_packing<spaces_type>(rectangles, make_finder_input(
                            max_side, discard_step, report_successful, report_unsuccessful,
                            runtime_flipping_mode
                        )
        );

        Index rectIndex = 0;
        for (auto& itex: texturesBySide) {
            auto& r = rectangles[rectIndex];
            auto& tex = textures_[itex.second];
            tex.targetRect = {r.x, r.y, r.w, r.h};
            markImagePlaced(tex);
            ++rectIndex;
        }
        for (auto& font: fonts_) {
            for (auto& glyph: font.glyphs) {
                if (glyph.phonyImage)
                    continue;
                auto& r = rectangles[rectIndex];
                glyph.targetRect = {r.x, r.y, r.w, r.h};
                markImagePlaced(glyph);
                ++rectIndex;
            }
        }

        //report_result(result_size);
    }

    //{
    //	Example 3: Find best packing exactly in the order of provided input. */

    //	const auto result_size = find_best_packing_dont_sort<spaces_type>(
    //		rectangles,
    //		make_finder_input(
    //			max_side,
    //			discard_step,
    //			report_successful,
    //			report_unsuccessful,
    //			runtime_flipping_mode
    //		)
    //		);

    //	Index rectIndex = 0;
    //	for (auto& itex : texturesBySide) {
    //		auto& r = rectangles[rectIndex];
    //		auto& tex = textures_[itex.second];
    //		tex.targetRect = { r.x + 1, r.y + 1, r.w - 2, r.h - 2 };
    //		tex.placed = true;
    //		++rectIndex;
    //	}
    //	for (auto& font : fonts_) {
    //		for (auto& glyph : font.glyphs) {
    //			if (glyph.phonyImage)
    //				continue;
    //			auto& r = rectangles[rectIndex];
    //			glyph.targetRect = { r.x + 1, r.y + 1, r.w - 2, r.h - 2 };
    //			glyph.placed = true;
    //			++rectIndex;
    //		}
    //	}

    //	report_result(result_size);
    //}


}
