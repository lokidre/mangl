/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "game_screen.h"

#include "state.hpp"

namespace toolkit {

void GameScreen::buildAnimation() {
    using namespace apl;

    auto state = State::instance();

    auto& projectFiles = state->projectFiles;

    auto& aniFiles = projectFiles.fileContainers[(int)ProjectCategory::Animation];

    if (aniFiles.selectedIndex < 0)
        return;

    auto& aniFile = aniFiles.files[aniFiles.selectedIndex];
    auto& filename = aniFile.path;


    auto scene = getScene();
    auto renderer = getRenderer();
    auto layout = Env::fullLayout();

    animation_.clear();
    // TODO: params
    //animation_.setParameters(params);

    animation_.preload(filename);
    auto animationTextureFile = animation_.atlasFile;

    // Set up the animation
    animation_.clear();
    //animation_.setParameters(params);

    auto viewport = layout;
    animation_.setViewport(viewport, Env::layoutPadding);
    animation_.load(filename);


    if (!animationTextureFile.empty() && animationTextureFile != animationTextureFile_) {
        String atlasFile;
        if (animationTextureFile.front() != ':') {
            auto dirname = extractDirName(filename);
            std::vector<String> dirs;
            if (!animation_.textureDir.empty())
                dirs.push_back(animation_.textureDir);
            dirs.emplace_back(".");

            for (auto& dir: dirs) {
                String finalDirname;

                // Two variations: relative and absolute paths
                if (dir == ".")
                    finalDirname = dirname;
                else if (dir.size() >= 2 && dir.substr(0, 2) == "..")
                    finalDirname = makeFilePath(dirname, dir);
                else
                    finalDirname = dir;

                atlasFile = makeFilePathExt(finalDirname, animationTextureFile, M::DefaultTextureAtlasExt);

                if (fileExists(atlasFile))
                    break;
            }
        } else {
            atlasFile = makeFileName(animationTextureFile, M::DefaultTextureAtlasExt);
        }

        if (!animationTex_.loaded(atlasFile))
            animationTex_.load(atlasFile);

        animation_.create(scene, &animationTex_);
    } else {
        // If animation has no
        animation_.create(scene, &guiTex_);  // For text purposes
    }

    animation_.start(GameScreenApp::timestamp());
}

}  // namespace toolkit
