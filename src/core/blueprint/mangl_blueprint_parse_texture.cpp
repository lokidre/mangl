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

void Loader::parseDocumentTexture(StrView filename, YamlDocA doc)
{
    String name, file;

    for (auto& root: doc) {
        switch (pshHash(root.key)) {
        case "name"_psh: name = root.getString(); break;
        case "file"_psh: file = root.getString(); break;
        default: raiseErrorInvalidKey(doc);
        }
    }

    verifySource(name, doc, "Texture name is missing");

    auto texture = blueprint_->textures.add(name);
    texture->name = name;
    texture->nameHash = TexturesContainer::hash(texture->name);
    texture->file = file;
}



}  // namespace mangl::blueprint
