/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "params.h"

#include <apeal/cmdline.hpp>
#include <apeal/parse.hpp>
#include <apeal/console.hpp>

static const char* helpHeader_ = R"(
Texture Generator Tool.
Copyright (C) 2014-2024 Andrei Mirzoyan

usage: mangl-make-tex [<options>...]

options:
)";


int TexMakeParams::parseCmdLine(int argc, char* argv[]) {
    using namespace apl;
    //using namespace std;

    CommandLine cmdline;

    //cmdline.bindRequiredArgument(&texmakeFile);
    cmdline.bindArguments(&outputFiles, 0, 3);

    cmdline.add(
        {.name = "help",
         .action =
             [&](auto) {
                 cmdline.printout(helpHeader_);
                 cmdline.stop();
             },
         .desc = "Print this message."}
    );
    cmdline.dup("h", "help");

    cmdline.add(
        {.name = "input",
         .sbind = &inputFile,
         .desc = "Texture description file."}
    );
    cmdline.dup("i", "input");

    String definesString;
    cmdline.add({.name = "define", .sbind = &definesString, .desc = "Defines: NAME=VALUE;NAME=VALUE"});


    cmdline.add(
        {.name = "output",
         .sbind = &outputFile,
         .desc = "Output atlas name. Image file name will be derived by using .png extension."}
    );
    cmdline.dup("o", "output");


    cmdline.add(
        {.name = "suffix",
         .sbind = &suffix,
         .desc = "Suffix to be used to added to source and target file names."}
    );
    cmdline.dup("u", "suffix");

    cmdline.add(
        {.name = "fallback-suffix",
         .sbind = &fallbackSuffix,
         .desc = "Suffix to fall back if files with --suffix parameters are not found."}
    );

    cmdline.add({.name = "deps", .sbind = &depsFile, .desc = "Dependencies file to generate."});
    cmdline.dup("d", "deps");

    cmdline.add(
        {.name = "scale", .ibind = &scale, .desc = "Assumed texture image scale. Default: 1."}
    );
    cmdline.dup("s", "scale");

    cmdline.add(
        {.name = "--density",
         .ibind = &density,
         .desc = "Texture density. The parameter goes straight to the atlas file. Default: 1."}
    );

    cmdline.add(
        {.name = "--bleeding",
         .ibind = &bleeding,
         .desc = "Bleed images by number of pixels. Default: 1."}
    );

    cmdline.add(
        {.name = "--scale-missing",
         .bbind = &scaleMissing,
         .desc = "Automatically scale missing images. Debugging only."}
    );

    auto retCode = cmdline.process(argc, argv);


    if (!scale) // if the scale parameter is missing, use 1
        scale = 1;
    if (!density) // if the density parameter is missing, then it equals to scale
        density = scale;

    // handle the output files
    verify(!outputFile.empty(), "--output -o is missing");
    verify(!inputFile.empty(), "--input -i is missing");

    std::vector<String> outfiles;

    auto outv = parse_std_vector_string_trim(outputFile, ' ');
    for (auto& f: outv)
        outfiles.push_back(f);

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

        throw std::runtime_error(fmt("Invalid output %s", f));
    }

    if (!definesString.empty()) {
        auto varPairs = parse_std_vector_string_trim(definesString, ';');
        for (auto& pair: varPairs) {
            auto [val, name] = str_split_trim(pair, '=');

            defines.insert({String{val}, String{name}});
        }
    }


    return retCode;
}
