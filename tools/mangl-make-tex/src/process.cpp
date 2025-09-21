/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "texmake.h"
#include <apeal/file.hpp>
#include <apeal/image.hpp>


void TexMake::initializePlacements() {
    auto outSize = outputImage_.getSize();

    imageMask_.alloc(outSize.w * outSize.h);
    imageMask_.zero();

    initializeDFS();
}

int TexMake::process()
{
    loadDescFile();

    if (!params.outputImageFile.empty()) {
        //
        // First pass.
        // Read and analyze every source image
        //
        for (auto& src: textures_) {
            if (src.phonyImage)
                continue;

            loadSourceImageInfo(src, src.filename, params.margin);
        }

        for (auto& font: fonts_) {
            if (font.type == FontSourceType::Image) {
                SizeI maxSize{};
                int div = 64;

                for (auto& glyph: font.glyphs) {
                    loadSourceImageInfo(glyph, glyph.filename, params.margin);
                    //inputImage = loadImage(glyph.filename);

                    auto sz = glyph.sourceRect.size();

                    //glyph.targetRect = glyph.sourceRect; //.setSize(sz);
                    //glyph.sourceRect.setSize(sz);

                    if (sz.w > maxSize.w) maxSize.w = sz.w;
                    if (sz.h > maxSize.h) maxSize.h = sz.h;

                    glyph.insertParams("bitmap_size", std::vector{sz.w, sz.h});
                    glyph.insertParams("bitmap_rect", std::vector{0, 0, sz.w, sz.h});
                    glyph.insertParams("bitmap_margin", std::vector{0, 0});
                    glyph.insertParams("glyph_size", std::vector{sz.w * div, sz.h * div});
                    glyph.insertParams("horz_bearing", std::vector{0, sz.h * div});
                    glyph.insertParams("vert_bearing", std::vector{0, 0});
                    glyph.insertParams("advance", std::vector{sz.w * div, sz.h * div});
                }


                font.insertParams("div", div);
                font.insertParams("height", maxSize.h);

                FontParamMapValue face;
                
                int unitsPerEm = 2048;

                insertParamMapValue("units_per_em", unitsPerEm, &face);
                //insertParamMapValue("bbox", std::vector<int>{-1126,2466,-555,2146}, &face);
                insertParamMapValue("bbox", std::vector{0, maxSize.w * div, 0, maxSize.h * div}, &face);
                insertParamMapValue("ascent", 2189, &face);
                insertParamMapValue("descent", -600, &face);
                insertParamMapValue("height", maxSize.h * div, &face);
                insertParamMapValue("max_advance", maxSize.w * div, &face);
                insertParamMapValue("underline_position", 0, &face);
                insertParamMapValue("underline_thickness", div * 2, &face);

                font.insertParams("face", face);


                FontParamMapValue metrics;
                insertParamMapValue("ppem", std::vector{32, 32}, &metrics);
                insertParamMapValue("scale", std::vector{65536, 65536}, &metrics);
                insertParamMapValue("ascent", 2240, &metrics);
                insertParamMapValue("descent", -640, &metrics);
                insertParamMapValue("height", maxSize.h * div, &metrics);
                insertParamMapValue("max_advance", maxSize.w * div, &metrics);

                font.insertParams("metrics", metrics);
            }
        }

        // Second pass.
        initializePlacements();



        // Calculate the images' placement
        switch (params.method) {
        using enum TexPlaceMethod;
        case Brute:
            if (!buildPlacementsBruteForce())
                apl::throwError("Not enough room for textures");
            break;
        case Dfs:
            if (!buildPlacementsDFS())
                if (!buildPlacementsBruteForce())
                    apl::throwError("Not enough room for textures");
            break;
        case Rectpack2D:
            buildPlacementsRectPack2D();
            break;
        }


        //
        // Handle the variables
        //
        texByVar_.clear();

        int index = 0;
        for (auto& tex: textures_)
            texByVar_.insert({tex.variableName, index++});

        // count number of occurances of each variable
        if (!texByVar_.empty()) {
            String lastVar;
            std::multimap<String, int>::iterator arrayFirstIt;
            int variableCount{};

            for (auto texIt = texByVar_.begin(), texEnd = texByVar_.end();; ++texIt) {
                index = (texIt != texEnd) ? texIt->second : 0;
                auto& tex = textures_[index];

                if (lastVar != tex.variableName || texIt == texEnd) {
                    if (!lastVar.empty()) {
                        for (auto t = arrayFirstIt; t != texIt; ++t) {
                            auto& rtex = textures_[t->second];
                            rtex.isArray = variableCount != 1;
                            rtex.arraySize = variableCount;
                        }
                    }
                    variableCount = 0;
                    arrayFirstIt = texIt;
                }

                if (texIt == texEnd)
                    break;

                tex.arrayIndex = variableCount;
                ++variableCount;
                lastVar = tex.variableName;
            }
        }


        // Build the atlas
        buildTextureDesc();

        // Build the target image.
        buildTextureImage();
    }

    // generate the dependency files (if necessary)
    if (!params.depsFile.empty() && !deps_.empty()) {
        buildDeps();
    }


    // Build the C++ header file
    if (!params.outputHeaderFile.empty()) {
        buildTextureHeader();
    }

    return 0;
}

bool TexMake::preprocessDefines(String& variable)
{
    bool processed = false;

    //
    // process defines
    //
    String destination;
    for (auto ch = variable.begin(), chEnd = variable.end(); ch != chEnd; ) {
        if (*ch == '$') {
            ++ch;
            if (ch == variable.end())
                break;

            if (*ch == '$') {
                destination += *ch;
                continue;
            }

            String defineName;

            if ('{' == *ch || *ch == '(') {

                char endChar = *ch == '{' ? '}' : ')';
                ++ch;
                for (; ch != chEnd && *ch != endChar; ++ch)
                    defineName += *ch;
                if (ch != chEnd)
                    ++ch;

            } else {  // If the variable is without bracket or braces
                for (; ch != chEnd && std::isalnum(*ch); ++ch) {
                    defineName += *ch;
                }
            }

            auto d_it = defines_.find(defineName);
            apl::verify(d_it != defines_.end(), "Unknown variable: %s", defineName);
            destination += d_it->second;

            processed = true;

        } else {
            destination += *ch++;
        }
    }
    variable = destination;
    return processed;
}


String TexMake::preprocessVariable(StrView varName, StrView varValue)
{
    if (auto varIt = params.defines.find(varName); varIt != params.defines.end()) {
        return varIt->second;
    }

    String var{varValue};
    int count = 0;

    while (preprocessDefines(var)) {
        count++;
        apl::verify(count < 25, "Too many define nesting: possibly cyclical");
    }

    return var;
}

String TexMake::processResourceFilename(StrView input)
{
    auto filename = preprocessVariable("__resource_file", input);

    if (filename[0] == '~') {
        String homeDir;
#if TARGET_OS_WINDOWS
        homeDir = "C:";  // TODO: Get user home from Windows
#else
        char* env = getenv("HOME");
        homeDir = env;
#endif

        filename.erase(0, 1);
        if (filename[0] != '/')
            filename.insert(0, 1, '/');
        filename.insert(0, homeDir);
    }

    return filename;
}


String TexMake::processOutputFilename(StrView input)
{
    auto filename = processResourceFilename(input);

    if (!params.suffix.empty()) {
        auto [sbase, ext] = apl::splitFileExt(filename);

        auto base = String{sbase} + params.suffix;

        filename = ext.empty() ? base : apl::makeFileName(base, ext);
    }

    return filename;
}

String TexMake::processInputFileName(StrView input, StrView defExt, bool nosuffix)
{
    using namespace apl;

    auto filename = processResourceFilename(input);

    auto [baseView, extView] = splitFileExt(filename);

    String base{baseView};
    String ext{extView.empty() ? defExt : extView};

    if (nosuffix || suffixes_.empty()) {
        auto name = makeFileName(base, ext);
        if (!fileExists(name))
            throwError("Cannot find input file: %s", name);
        return name;
    }


    for (auto& suffix: suffixes_) {

        //
        // Try this format: name-sfx.ext
        //
        auto name = makeFileName(base + suffix, ext);

        if (fileExists(name))
            return name;


        //
        // Try this format: name-sfx_xxxx.ext
        // where xxx are any number of decimal digits
        //
        auto idx = base.size();

        // skip the digits
        while (idx > 0 && std::isdigit(base[idx-1]))
            --idx;

        // skip the underscore
        if (idx > 0 && base[idx-1] == '_')
            --idx;

        name = fmt("%s%s%s", base.substr(0, idx), suffix, base.substr(idx));
        name = makeFileName(name, ext);

        // Test it
        if (fileExists(name))
            return name;
    }

    if (!suffixes_.empty()) {
        // If we are here then all the tests failed
        // Bail out
        throwError("Cannot find input file: %s with any suffixes: %s", filename, suffixes_);
    }

    return filename;
}

