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

void Engine::create(SceneP scene)
{
    using namespace apl;
    
    scene_ = scene;


    //void Loader::createScreenComponents(Index screenIndex, const TextureAtlas & gameTex) {
    //auto scene = App::instance()->scene();

    //TextureAtlas gameTex;
    gameTex.load("C:\\Users\\Arus\\Dropbox\\apps\\pentix\\textures\\320x480\\tex_gui");  //TODO:gameTex from parameters

    /*
    auto& project = blueprint_;
    auto& screen = blueprint_.screens[0];
    for (auto& w : screen.worlds) {
        for (auto& elem : w.components) {
            switch (pshHash(elem.type)) {
            case"rectangle"_psh: {
                auto r = calcCoords(elem, screen);
                //auto c = M::White;
                auto c = elem.color[0];
                auto prim = scene->newRect(c, r);
                elem.componentLayout.x = r.x;
                elem.componentLayout.y = r.y;

                if (elem.border.size) {
                    prim->setWidth(elem.border.size);
                }
                break;
            }
            case"label"_psh: {
                auto& font = gameTex.getFont(elem.name);
                auto p = calcPointCoords(elem, screen);
                auto textHeight = elem.componentLayout.height.value;
                auto labelColor = M::Silver;
                //Color textColor{M::White};
                auto textColor = elem.color[0];
                elem.componentLayout.x = p.x;
                elem.componentLayout.y = p.y;

                scene_->newLabel(elem.text, p, textHeight, textColor, M::AlignCenterAll)->setFont(&font);
                break;
            }
            case"image"_psh: {
                auto& image = gameTex.get(elem.name);
                auto r = calcCoords(elem, screen);
                scene_->newImage(r, image);
                elem.componentLayout.x = r.x;
                elem.componentLayout.y = r.y;
                break;
            }
            case"button"_psh: {
                auto r = calcCoords(elem, screen);
                auto& button = gameTex.get(elem.name);
                scene_->newButton(button, r);
                elem.componentLayout.x = r.x;
                elem.componentLayout.y = r.y;

                break;
            }

            default:
                break;

            }
        }
    }
     */


    //}






    //for (auto&& node: blueprint_.orderedNodes) {
    //    auto&& nodeRect = node->evalRect();

    //    switch (node->type) {
    //    case NodeType::Unknown:
    //        break;

    //    case NodeType::BoxLayout:
    //        evalNodeRect(node);
    //        node->prim = scene->newBox(nodeRect);
    //        updateNodePrimColors(node);
    //        break;

    //    case NodeType::Image: {
    //        auto& textures = blueprint_.textures;
    //        if (node->atlasName.empty())
    //            node->atlasRef = textures.begin()->second;
    //        else
    //            node->atlasRef = textures.get(node->atlasName);

    //        verify(node->atlasRef, "Atlas not found: %s", node->atlasName);

    //        verify(node->atlasRef->atlas, "Atlas not set up: %s", node->atlasName);

    //        node->texture = node->atlasRef->atlas->get(node->textureName);

    //        evalNodeRectRatioed(node, node->texture.size());
    //        Size elSize{};
    //        if (node->elementSize.valid) {
    //        }

    //        node->prim = scene->newImage(nodeRect, node->texture, elSize);

    //        updateNodePrimColors(node);

    //        //if (node->colors.front().valid)
    //        //    node->prim->setColor(node->colors.front().valid);
    //        break;
    //    }
    //    default:
    //        ADebugTrap();
    //        break;
    //    }

    //}

    created_ = true;

    updateDebugStatus();
}

}  // namespace mangl::blueprint
