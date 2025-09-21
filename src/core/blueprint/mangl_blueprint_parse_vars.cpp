/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_blueprint_loader.h"

#include "../mangl_parse.h"

namespace mangl::blueprint {

using namespace apl;
using namespace apl::literals;


void Loader::parseHeaderVars(YamlNodeA yaml) {
    VariableP var{};

    for (auto& vel: yaml.nodes) {
        if (pshHash(vel.key) == "name"_psh) {
            //            auto name = el.getScalar();
            auto name = vel.getValue();
            var = blueprint_->variables.add(name);
            var->name = name;
            for (auto&& el: vel.nodes) {
                switch (pshHash(el.key)) {
                case "value"_psh: var->value.setString(el.value); break;
                case "name"_psh: break;
                case "comment"_psh:
                case "notes"_psh: break;
                default: raiseErrorInvalidKey(yaml);
                }
            }
            // break;
        }
    }

    // if (!var)
    //     return raiseParseError(yaml, "Missing Variable Name");

    //// first get the variable name
    // for (auto& el: yaml.nodes) {
    //     switch (pshHash(el.key)) {
    //     case "value"_psh: var->value.setString(el.value); break;
    //     case "name"_psh: break;
    //     case "comment"_psh:
    //     case "notes"_psh: break;
    //     default: raiseErrorInvalidKey(yaml);
    //     }
    // }
}


} // namespace mangl::blueprint
