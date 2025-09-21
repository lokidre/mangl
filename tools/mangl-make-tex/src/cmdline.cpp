/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "params.h"

#include "../version.hpp"

#include <apeal/cmdline.hpp>
#include <apeal/parse.hpp>
#include <apeal/console.hpp>

static auto toolName_ = "mangl-make-tex";

static auto helpHeader_ = R"(
Mangl Texture Generator Tool.
Copyright (C) 2014-2024 Andrei Mirzoyan

usage: mangl-make-tex [<options>...]

options:
)";


int TexMakeParams::parseCmdLine(int argc, char* argv[]) {
    using namespace apl;

    CommandLine cmdline;

    cmdline.bindArguments(&outputFiles, 0, 3);

    cmdline.add({
        .name = "help",
        .salt = 'h',
        .action = [&](auto) {
             cmdline.printout(helpHeader_).stop();
         },
        .desc = "Print this message."
    });

    cmdline.add({
        .name = "version",
        .salt = 'v',
        .action = [&](auto) {
             cmdline.print(fmt("%s %s", toolName_, version_)).stop();
         },
        .desc = "Print version."
    });

    cmdline.add({
        .name = "input",
        .salt = 'i',
        .sbind = &inputFile,
        .desc = "Texture description file."
    });

    String definesString;
    cmdline.add({
        .name = "define",
        .sbind = &definesString,
        .desc = "Defines: NAME=VALUE;NAME=VALUE"
    });


    cmdline.add({
        .name = "output",
        .salt = 'o',
        .sbind = &outputFile,
        .desc = "Output atlas name. Image file name will be derived by using .png extension."
    });

    cmdline.add({
        .name = "suffix",
        .salt = 'u',
        .sbind = &suffix,
        .desc = "Suffix to be used to added to source and target file names."
    });

    cmdline.add({
        .name = "fallback-suffix",
        .sbind = &fallbackSuffix,
        .desc = "Suffix to fall back if files with --suffix parameters are not found."
    });

    cmdline.add({
        .name = "deps",
        .salt = 'd',
        .sbind = &depsFile,
        .desc = "Dependencies file to generate."
    });

    cmdline.add({
        .name = "scale",
        .salt = 's',
        .ibind = &scale,
        .desc = "Assumed texture image scale. Default: 1."
    });

    cmdline.add({
        .name = "--density",
        .ibind = &density,
        .desc = "Texture density. The parameter goes straight to the atlas file. Default: 1."
    });

    cmdline.add({
        .name = "--margin",
        .ibind = &margin,
        .desc = "Specifies gap between the textures. Default: 1."
    });

    cmdline.add({
        .name = "--bleed",
        .bbind = &bleed,
        .desc = "Bleed images by number of pixels. Default: true."
    });

    cmdline.add({
        .name = "--bleeding-debug",
        .bbind = &bleedingDebug,
        .desc = "Render debugging color for bleeding areas. Default: false."
    });

    cmdline.add({
        .name = "--scale-missing",
        .bbind = &scaleMissing,
        .desc = "Automatically scale missing images. Debugging only."
    });

    String methodString;
    cmdline.add({
        .name = "--method",
        .sbind = &methodString,
        .desc = "Method to place the images: [brute, dfs]."
    });

    auto retCode = cmdline.process(argc, argv);

    if (cmdline.stopped())
        return retCode;


    if (!methodString.empty()) {
        using enum TexPlaceMethod;
        if (methodString == "brute") method = Brute; else
        if (methodString == "dfs") method = Dfs; else
        if (methodString == "rectpack") method = Rectpack2D; else
            throwError("Unknown method: %s", methodString);
    }

    if (!scale) // if the scale parameter is missing, use 1
        scale = 1;
    if (!density) // if the density parameter is missing, then it equals to scale
        density = scale;

    // handle the output files
    verify(!outputFile.empty(), "--output -o is missing");
    verify(!inputFile.empty(), "--input -i is missing");

    auto outfiles = parseStdVectorStringTrim(outputFile, ' ');

    for (auto& f: outputFiles)
        outfiles.push_back(f);

    for (auto& f: outfiles) {
        auto [name, ext] = splitFileExt(f);

        if (ext.empty()) {
            outputImageFile = makeFileName(name, "png");
            outputDescFile = makeFileName(name, "yaml");
            continue;
        }

        if (ext == "png") {
            outputImageFile = f;
            continue;
        }

        if (ext == "yaml") {
            outputDescFile = f;
            continue;
        }
        if (ext == "hpp" || ext == "h") {
            outputHeaderFile = f;
            continue;
        }

        throwError("Invalid output %s", f);
    }

    if (!definesString.empty()) {
        auto varPairs = parseStdVectorStringTrim(definesString, ';');
        for (auto& pair: varPairs) {
            auto [val, name] = strSplitTrim(pair, '=');

            defines.insert({String{val}, String{name}});
        }
    }


    return retCode;
}
