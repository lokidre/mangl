/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_texture.h"

#include <apeal/parse.hpp>

namespace mangl {

using FontCharCode = unsigned;

struct FontGlyph {
    FontCharCode charcode{};
    Size glyphSize{};
    Point bitmapMargin{};
    Point horzBearing{}, vertBearing{}, advance{};
    Texture texture{};
    KeyValCont<FontCharCode, Cord> kerning;
};


struct Font {
    using CharCode = FontCharCode;
    using GlyphContainer = KeyValCont<FontCharCode, FontGlyph>;

    TextureId textureId{Texture::invalidId()};

    Cord textureDensity{1};

    bool variableWidth{};
    Cord spaceAdvance{};

    GlyphContainer charlist;

    // Various data loaded from font file
    Cord unitsPerEM{};
    Cord bboxXMin{}, bboxXMax{}, bboxYMin{}, bboxYMax{};
    Cord ascent{}, descent{};
    Cord designHeight{}, designMaxAdvance{};
    Cord underlinePosition{}, underlineThickness{};
    Point ppem{}, metricsScale{};
    Cord metricsAscent{}, metricsDescent{};
    Cord metricsHeight{};
    Cord metricsMaxAdvance{};

    Cord fontDiv{}, fontHeight{};


    void insertGlyph(FontGlyph&& glyph) {
        charlist.emplace_hint(charlist.end(), glyph.charcode, glyph);
    }

    bool valid() const noexcept { return Texture::validId(textureId); }

    auto& getGlyphs() const noexcept { return charlist; }

    auto getSpaceAdvance() const noexcept { return spaceAdvance; }
    auto getDensity() const noexcept { return textureDensity; }


    // Querying supported char set
    bool isCharsetSupported(StrView text) const noexcept;

    // Strange: shouldn't be YMax - YMin?
    auto bboxHeight() const { return bboxYMax + bboxYMin; }


    void setDefault() const;


    static const Font *getDefaultFont() noexcept;
};

using FontP = Font*;
using FontA = const Font&;
using FontP = Font*;
using FontCP = const Font*;

namespace M {
//
// Utilities
//
inline
FontCharCode fontCharCodeParse(StrView str) {
    using namespace apl;
    verify(!str.empty(), "Empty charcode");

    auto it = str.begin(), end = str.end();

    if (*it != 'U')
        return parseUIntIt(it, end);

    ++it;
    if (*it == '+')
        ++it;
    return parseUIntHexIt(it, end);
};

inline
String fontCharCodeFormat(FontCharCode code, bool unicode = true, bool canonic = false) {
    using namespace apl;
    verify(code, "Invalid character code");
    return unicode ? fmt(canonic ? "U+%04X" : "U%04X", code) : fmt("%d", code);
}

}  // namespace M

} // namespace mangl

