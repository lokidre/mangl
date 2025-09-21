/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "font_renderer.h"

#include <apeal/cmdline.hpp>
#include <apeal/parse.hpp>
#include <apeal/console.hpp>

static const char* helpHeader_ = R"(
Generate Font Character Textures.
Copyright (C) 2014-2023 Andrei Mirzoyan

mangl-font-render [<options>...] <font-file>

options:
)";


int FontRendererParams::parseCmdLine(int argc, char* argv[]) {
    using namespace apl;

    CommandLine cmdline;

    cmdline.bindArguments(&outputFiles, 0, 2);
    //cmdline.bindRequiredArgument(&fontFile);


    cmdline.addFlag({
        .name = "--help",
        .salt = 'h',
        .action = [&] { cmdline.printout(helpHeader_).stop(); },
        .desc = "Print this message."
    });


    cmdline.addFlag({
        .name = "--info",
        .bind = &infoOnly,
        .desc = "Show only information about the font file"
    });

    cmdline.add({
        .name = "input",
        .salt = 'i',
        .sbind = &fontFile,
        .desc = "Font description file."
    });

    cmdline.add(
        {.name = "--output",
         .sbind = &outputFile,
         .desc =
             "Font description file. Will generate together with .png files for every character"}
    );
    cmdline.dup("-o", "output");

//    cmdline.add(
//        {.name = "--suffix",
//         .sbind = &outputSuffix,
//         .desc = "Suffix to be used to added to source and target file names."}
//    );
//    cmdline.dup("-u", "suffix");

    cmdline.add({
        .name = "scale",
        .salt = 's',
        .ibind = &scale, .desc = "Assumed font image scale. Default: 1."
    });

    cmdline.add({.name = "size", .ibind = &fontSize, .desc = "Font size. Default: 64."});
    cmdline.dup("height", "size");


    cmdline.add({
        .name = "--charset",
        .salt = 'c',
        .action =
             [this](auto v) {
                 using namespace apl;

                 auto [sbegin, send] = str_split_trim(v, '-');
                 verify(!sbegin.empty() && !send.empty(), "Invalid charset range: %s", v);

                 auto ibegin = mangl::M::fontCharCodeParse(sbegin);
                 auto iend = mangl::M::fontCharCodeParse(send);

                 verify(ibegin && iend, "Invalid range values: %s", v);

                 characterRanges.push_back({ibegin, iend});
             },
        .desc = "<begin>-<end>  Provides character range. Default: 32-127"
    });

    auto retcode = cmdline.process(argc, argv);

    // Some some more checks if we have to

    verify(!outputFile.empty(), "--output -o is missing");
    verify(!fontFile.empty(), "--input -i is missing");


    std::vector<String> outfiles;

    auto outv = parse_std_vector_string_trim(outputFile, ' ');
    for (auto& f: outv)
        outfiles.push_back(f);

    for (auto& f: outputFiles)
        outfiles.push_back(f);

    for (auto& f: outfiles) {
        auto [name, ext] = splitFileExt(f);

        if (ext.empty()) {
            outputImageFile = makeFilePath(name, "png");
            outputDescFile = makeFilePath(name, "yaml");
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

        throw std::runtime_error(fmt("Invalid output %s", f));
    }


    return retcode;
}
