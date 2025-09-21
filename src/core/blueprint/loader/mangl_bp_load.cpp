/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_bp_loader.h"

#include "../mangl_blueprint.h"


namespace mangl::bp {

void Loader::load(StrView data, Blueprint* bp) {
    blueprint_ = bp;



    auto yaml = apl::parseYaml(data);


    for (auto& node: yaml.front()) {
        using namespace apl;

        switch(pshHash(node.key)) {
        case "project"_psh:
            verify(!bp->project, "Project is already defined");
            bp->project = new Project();
            loadProject(node, bp->project);
            break;
        default: throwError("Invalid root key: '%s'", node.key);
        }

    }

}

void Loader::loadProject(YamlNodeA projectNode, Project* project) {
    using namespace apl;

    for (auto& node: projectNode) {
        switch(pshHash(node.key)) {
        case "id"_psh: node.get(&project->id); break;
        case "title"_psh: node.get(&project->title); break;
        case "screens"_psh:
            for (auto& screenNode: node) {
                String screenId;
                screenNode.getProp("id", &screenId);
                auto screen = blueprint_->project->addScreen(screenId);
                loadScreen(screenNode, screen);

            }
            break;
        default: throwError("Invalid project key: '%s'", node.key);
        }
    }
}

void Loader::loadScreen(YamlNodeA screenNode, Screen* screen) {
    using namespace apl;
    for (auto& prop: screenNode) {
        switch (pshHash(prop.key)) {
        case "id"_psh: break;

        default: throwError("Invalid screen key: '%s'", prop.key);
        }
    }
}


}
