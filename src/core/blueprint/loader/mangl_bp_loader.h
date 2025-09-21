/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include <mangl/mangl.h>

#include <apeal/parse/yaml.hpp>

#include "../project/mangl_bp_project.h"

namespace mangl {
class Blueprint;
}

namespace mangl::bp {

class Loader {

    using YamlDoc = apl::Yaml::Document;
    using YamlDocA = const YamlDoc&;

    using YamlNode = apl::Yaml::Node;
    using YamlNodeA = const YamlNode&;


    Blueprint* blueprint_{};

public:
    void load(StrView data, Blueprint* bp);


private:

    void loadProject(YamlNodeA node, Project* project);
    void loadScreen(YamlNodeA node, Screen* screen);

};

}

