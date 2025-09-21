/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_parse.h"

#include <apeal/parse/parse_base.hpp>

#include <cctype>

namespace mangl {

static std::pair<StrView, uint32_t> colorNames[] {
    {"white",        0xffffff},
    {"whitesmoke",   0xf5f5f5},
    {"gainsboro",    0xdcdcdc},
    {"lightgray",    0xd3d3d3},
    {"lightgrey",    0xd3d3d3},
    {"silver",       0xc0c0c0},
    {"darkgray",     0xa9a9a9},
    {"darkgrey",     0xa9a9a9},
    {"gray",         0x808080},
    {"grey",         0x808080},
    {"dimgray",      0x696969},
    {"dimgrey",      0x696969},
    {"black",        0x000000},


    {"red",          0xff0000},
    {"crimson",      0xdc143c},
    {"firebrick",    0xb22222},
    {"brown",        0xa52a2a},
    {"darkred",      0x8b0000},
    {"maroon",       0x800000},

    {"ivory",        0xfffff0},
    {"lightyellow",  0xffffe0},
    {"yellow",       0xffff00},
    {"gold",         0xffd700},
    {"orange",       0xffa500},
    {"darkorange",   0xff8c00},
    {"olive",        0x808000},

    {"lime",         0x00ff00},
    {"springgreen",  0x00ff7f},
    {"palegreen",    0x98fb98},
    {"lightgreen",   0x90ee90},
    {"yellowgreen",  0x9acd32},
    {"limegreen",    0x32cd32},
    {"seagreen",     0x2e8b57},
    {"forestgreen",  0x228b22},
    {"green",        0x008000},
    {"darkgreen",    0x006400},

    {"aqua",         0x00ffff},
    {"darkcyan",     0x008b8b},
    {"teal",         0x008080},

    {"blue",         0x0000ff},
    {"deepskyblue",  0x00bfff},
    {"lightblue",    0xadd8e6},
    {"darkblue",     0x00008b},
    {"dodgerblue",   0x1e90ff},
    {"lightskyblue", 0x87cefa},
    {"mediumblue",   0x0000cd},
    {"mediumstateblue", 0x7b68ee},
    {"midnightblue", 0x191970},
    {"navy",         0x000080},

    {"fuchsia",     0xff00ff},
    {"magenta",     0xff00ff},
    {"purple",      0x800080},

};

Color parseColor(StrView text)
{
    using namespace apl;

    verify(!text.empty(), "Color value is blank");

    auto it = text.begin(), end = text.end();

    while (it != end && ParserBase::isSpace(*it))
          ++it;

    verify(it != end, "Color value is blank");

    // color is given by the number
    if (*it == '0' || *it == '#') {
        bool isHash = *it == '#';

        ++it;
        if (it == end)
            return {0}; // just a black color

       bool hex = isHash;

       if (!isHash && (*it == 'x' || *it == 'X')) {
          hex = true;
          ++it;
       }

        // now count the digits
        int count{};
        auto numBegin = it;

        while (it != end && ParserBase::isHexDigit(*it)) {
             ++it;
             ++count;
        }

        auto numEnd = it;
        uint32_t val{};
        parse_uint_radix_it(numBegin, numEnd, val, hex ? 16 : 0);
        Color color;

        if (count > 6)
          color.setRGBA(val);
        else
          color.setRGB(val);

        return color;
    }

    // prepare the name and make it lowercase
    String name;
    for (; it != end && ParserBase::isAlpha(*it); ++it)
        name += (char)std::tolower(*it);

    for (auto& tab: colorNames) {
        if (tab.first == name)
           return Color{tab.second};
    }


    throwError("Failed to parse color");
}

}  // namespace mangl
