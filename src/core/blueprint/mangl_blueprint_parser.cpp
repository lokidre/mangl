/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#include "mangl_blueprint_loader.h"

#include "../mangl_parse.h"

#include <apeal/parse/vector.hpp>

namespace mangl::blueprint {

using namespace apl;
using namespace apl::literals;

void Loader::parseModuleTags(StrView filename, YamlNodeA yaml, ModuleP mod, DepP parent) {
    switch (pshHash(yaml.tag.name)) {
    case "Header"_psh: parseDocumentHeader(mod->filename, yaml, mod, parent); break;
    case "Texture"_psh: parseDocumentTexture(mod->filename, yaml); break;
    case "Node"_psh: parseDocumentNode(mod->filename, yaml); break;
    default:
        for (auto& el : yaml) {
            if (el.tag.isCustom()) {
                parseModuleTags(filename, el, mod, parent);
            } else {
                raiseErrorInvalidKey(el);
            }
        }
        break;
    }
}


void Loader::parseDocumentHeader(StrView filename, YamlDocA doc, ModuleP mod, DepP parent) {
    for (auto& root : doc) {
        switch (pshHash(root.key)) {
        case "imports"_psh: parseHeaderImports(filename, root, mod, parent); break;
        case "vars"_psh: parseHeaderVars(root); break;
        default: raiseErrorInvalidKey(root);
        }
    }
}


} // namespace mangl::blueprint
