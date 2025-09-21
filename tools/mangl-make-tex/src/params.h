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

enum class TexPlaceMethod {
    Brute,
    Dfs,
    Rectpack2D,
};

struct string_hash {
    using hash_type = std::hash<std::string_view>;
    using is_transparent = void;

    std::size_t operator()(const char* str) const        { return hash_type{}(str); }
    std::size_t operator()(std::string_view str) const   { return hash_type{}(str); }
    std::size_t operator()(std::string const& str) const { return hash_type{}(str); }
};

struct TexMakeParams {
    String suffix, fallbackSuffix, resolution;
    String depsFile;  // dependencies file

    //std::unordered_map<String, String> defines;
    std::unordered_map<String, String, string_hash, std::equal_to<>> defines;

    //TexPlaceMethod method{TexPlaceMethod::Dfs};
    TexPlaceMethod method{TexPlaceMethod::Brute};

    int scale = 0;

    int margin = 1;  // Margin between textures
    bool bleed = true;  // Bleed one pixel
    bool bleedingDebug{};

    bool scaleMissing = false;
    int density = 0;

    String inputFile;

    std::vector<String> outputFiles;
    String outputFile;

    String outputImageFile, outputDescFile, outputHeaderFile;

    int parseCmdLine(int argc, char* argv[]);
};
