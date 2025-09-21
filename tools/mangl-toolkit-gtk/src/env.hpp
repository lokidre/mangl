/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_config.h"
#include "main_types.hpp"
#include <apeal/singleton.hpp>
#include <apeal/fs.hpp>

namespace toolkit {

class Env: public apl::Singleton<Env> {

public:
    String homeDir;
    String cacheDir;
    String stateFile;
    String settingsFile;
    //String

    void load() {
        using namespace apl;
        homeDir = FileSystem::getHomeDir();
        cacheDir = makeFilePath(homeDir, ".cache", MANGL_APP_TAG);
        FileSystem::mkdirIfNotExistsParents(cacheDir);

        stateFile = makeFilePath(cacheDir, "state.yaml");
        settingsFile = makeFilePath(cacheDir, "settings.yaml");


    }

    void saveState() {
        
    }

    void loadState() {
    }

};

}  // namespace toolkit
