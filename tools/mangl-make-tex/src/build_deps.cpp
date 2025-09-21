/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include <apeal/console.hpp>

#include "texmake.h"
#include <apeal/fs.hpp>

void TexMake::buildDeps() {

    using namespace apl;

    // First check the directory
    // this time make it
    auto [dir, name] = splitFilePath(params.depsFile);

    // Console::log("Building dependencies: %s", params.depsFile);

    if (!dir.empty() && dir != "." && dir != "..")
        AFS::mkdirIfNotExistsParents(dir);


    FileWriteFormat fdeps{params.depsFile};

    // formatting shortcuts
    // auto src = [&](StringA file) { deps.writeln("  %s", file); };
    // auto dst = [&](StringA file) { deps.writeln("%s:", file); };
    // auto srcm = [&](StringA file) { deps.writeln("  %s\\", file); };
    //auto dstm = [&](StringA file) { deps.writeln("%s:\\", file); };


    //deps.writeln("#");
    //deps.writeln("# Automatically generate file by: mangl-make-tex");
    //deps.writeln("#");
    //deps.nl();

    // list all the dependencies
    fdeps.writeln("%s:\\", params.outputDescFile);
    for (auto& file: deps_) fdeps.writeln("  %s\\", file);
    fdeps.nl();

    // The texture description file itself
    //srcm(params.inputFile);

    // // Every texture and font
    // for (auto& tex: textures_)
    //     if (!tex.phonyImage)
    //         srcm(tex.filename);
    // for (auto& font: fonts_)
    //     srcm(font.filename);


    // Now do the trick where make shows an error if some dependency doesn't exist,
    // but it doesn't needed anymore or will be generated
    // Every texture and font
    for (auto& file: deps_) fdeps.writeln("%s:", file);
    // for (auto& tex: textures_)
    //     if (!tex.phonyImage)
    //         dst(tex.filename);
    // for (auto& font: fonts_)
    //     dst(font.filename);

    fdeps.nl(); // Done
}
