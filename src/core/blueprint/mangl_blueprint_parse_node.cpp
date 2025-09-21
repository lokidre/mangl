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


void Loader::parseDocumentNode(StrView filename, YamlDocA doc)
{
    NodeP node{};

    // First pass
    for (auto& root: doc) {
        if (pshHash(root.key) == "id"_psh) {
            auto id = root.getScalar();
            auto& nodes = blueprint_->nodes;
            verifySource(!nodes.contains(id), doc, "Duplicate node id: %s", id);

            node = nodes.add(id);
            node->id = id;
            blueprint_->orderedNodes.push_back(node);
            break;
        }
    }

    if (!node)
        return raiseParseError(doc, "Node ID is missing");

    // Second pass
    for (auto& root: doc) {
        switch (pshHash(root.key)) {
        case "origin"_psh: node->origin = parseOrigin(root, node);  break;
        case "align"_psh: node->align = parseAlign(root, node);  break;
        case "pos"_psh:
        case "position"_psh: node->position = parseNodePosition(root, node); break;
        case "size"_psh: node->size = parseNodeSize(root, node); break;
        case "color"_psh: //node->colors[0] = parseLayoutColor(root, node); break;
        case "colors"_psh: node->numColors = parseLayoutColors(root, node, node->colors); break;

        case "image"_psh:
        case "texture"_psh: processNodeTypeImage(root, node); break;
        case "box"_psh: processNodeTypeBox(root, node); break;

        case "notes"_psh: break;  // remember the notes in the future
        case "id"_psh: break;  // already processed
        default: raiseErrorInvalidKey(root); break;
        }
    }
}

void Loader::processNodeTypeBox(YamlNodeA yaml, NodeP node)
{
    node->type = NodeType::Box;

    if (!yaml.isNull()) {
        auto comps = parseStdVectorTrim<StrView>(yaml.value, ',');
        auto numComps = comps.size();
        verifySource(numComps == 2 || numComps == 4, yaml, "BoxLayout components must be 2 or 4");

        node->position.x = parseLayoutMeasureHorz(yaml, comps[0], node);
        node->position.y = parseLayoutMeasureVert(yaml, comps[1], node);

        if (comps.size() == 4) {
            node->size.w = parseLayoutMeasureHorz(yaml, comps[2], node);
            node->size.h = parseLayoutMeasureVert(yaml, comps[3], node);
        }

    }

    for (auto& el: yaml) {
        switch (pshHash(el.key)) {
        case "size"_psh: node->size = parseNodeSize(el, node); break;
        default:
            break;
        }
    }

}

void Loader::processNodeTypeImage(YamlNodeA yaml, NodeP node)
{
    node->type = NodeType::Image;

    if (!yaml.isNull()) {
        auto texComps = str_split_trim(yaml.value, '.');

        auto& textures = blueprint_->textures;
        verify(!textures.empty(), "No textures set up");

        if (texComps.second.empty()) {
            verify(textures.size() > 1, "Texture name is required if more than one textures used");
            node->textureName = texComps.first;
        } else {
            node->atlasName = texComps.first;
            node->textureName = texComps.second;
        }

        verifySource(node->textureName, yaml, "Texture name is missing");
    }
}


}  // namespace mangl::blueprint
