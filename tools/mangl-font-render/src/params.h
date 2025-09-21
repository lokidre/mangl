/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "types.h"

struct FontRendererParams {
    String fontFile;
    bool infoOnly{};
    int fontSize{64};
    int scale{1};
    int gap{4};

    std::vector<std::pair<unsigned, unsigned>> characterRanges;

    std::vector<String> outputFiles;
    String outputFile;


    String outputImageFile, outputDescFile;


    int parseCmdLine(int argc, char* argv[]);

};

