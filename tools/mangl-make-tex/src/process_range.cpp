/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"

#include <apeal/parse.hpp>

StringList TexMake::processRange(StrView value)
{
    using namespace apl;

    StringList range;

    auto [rangeLeft, rangeContents] = str_split_trim(value, '[');

    // If Range wasn't provided
    if (rangeContents.empty()) {

        // Check extension
        auto [name, ext] = splitFileExt(value);
        range.emplace_back(value);

        if (ext.empty())
            range.back() += ".png";

        return range;
    }

    int rangeStepVal = 1;
    //int rangeFormatLength = 1 ;
    bool rangeFormatZeroes = true;

    auto [rangeBody, rangeRightView] = str_split_trim(rangeContents, ']');

    String rangeRight = rangeRightView.empty() ? ".png" : String{rangeRightView};

    auto [rangeFormat, rangeStep] = str_split_trim(rangeBody, ':');
    if (!rangeStep.empty())
        rangeStepVal = parseInt(rangeStep);

    auto [rangeBegin, rangeEnd] = str_split_trim(rangeFormat, '-');
    int rangeBeginVal = parseInt(rangeBegin);
    int rangeEndVal = parseInt(rangeEnd);

    rangeFormatZeroes = rangeBegin[0] == '0';
    auto rangeFormatLength = rangeBegin.size();
    auto rangeFinalFormat = fmt("%%s%%0%dd%%s", rangeFormatLength);

    for (int fileIdx = rangeBeginVal; fileIdx <= rangeEndVal; fileIdx += rangeStepVal) {
        range.emplace_back(fmt(rangeFinalFormat, rangeLeft, fileIdx, rangeRight));
    }

    return range;
}

