/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_blueprint.h"

namespace mangl::blueprint {

void Engine::updateNodePrimColors(Node* node)
{
    switch (node->numColors) {
    case 1:
        node->prim->setColor(node->colors[0].value);
        break;
    case 2:
        node->prim->setColors(node->colors[0].value, 
            node->colors[1].value);
        break;
    case 3:
        node->prim->setColors(node->colors[0].value, 
            node->colors[1].value,
            node->colors[2].value);
        break;
    case 4:
        node->prim->setColors(node->colors[0].value, 
            node->colors[1].value,
            node->colors[2].value,
            node->colors[3].value);
        break;
    default:
        break;
    }

}

}  // namespace mangl::blueprint

