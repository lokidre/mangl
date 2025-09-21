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

void Loader::parseHeaderImports(StrView filename, YamlNodeA yaml, ModuleP mod, DepP parent)
{
    /*
    for (auto& el: yaml.nodes) {
        auto path = locateModule(el.key, parent);

        // Check for recursive dependencies
        verifySource(!deps_.contains(path), yaml, "Recursive deps detected: %s", filename);

        auto dep = deps_.add(path);
        dep->path = path;
        dep->from = parent;
        dep->line = el.line;

        mod->imports.emplace_back(parseModule(path, dep->from));
    }
    */
}

}  // namespace mangl::blueprint
