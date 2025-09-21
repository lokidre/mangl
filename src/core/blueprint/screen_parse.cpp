/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/

#include <mangl/mangl.h>
#include <mangl/texture.h>
#include <mangl/widgets.h>

#include <apeal/parse/ini.hpp>
#include <apeal/parse/yaml.hpp>
#include <apeal/file.hpp>
#include <apeal/hash.hpp>
#include <any>
#include <vector>

#include "screen_parse.h"


namespace mangl {

using namespace apl;

//Color parseColor(String color) {
//    return apl::parseUIntHex(color);
//}
//
//CoordMajor parseCoords(String str, RealGL max ) {
//    CoordMajor obj{};
//    if ('%' == str.back()) {
//        auto val = str.substr(0, str.length() - 1);
//        obj.value = (max * stoi(val)) / 100;
//        obj.units = CoordUnit::Percent;
//        obj.max = max;
//    }
//    else {
//        obj.value = stoi(str);
//        obj.max = max;
//    }
//    return obj;
//}
//
//Project loadProject(String filename) {
//    Project project;
//    //std::set<String> fontNames;
//    auto yaml = loadYaml(filename);
//    //std::map<std::string, std::any> screen; vector , each elem with id
//    
//    for (auto& doc : yaml.documents) {
//        for (auto& docNode : doc) {
//            if ("project" == docNode.key) {
//                //Project project;
//                for (auto& projectNode : docNode) {
//                    for (auto& projectEl : projectNode) {
//                        switch (pshHash(projectEl.key)) {
//                        case"id"_psh: project.id = projectEl.getString(); break;
//                        case"title"_psh: project.title = projectEl.getString(); break;
//                        case"screenLayout"_psh: {
//                            for (auto& layoutEl : projectEl) {
//                                switch (pshHash(layoutEl.key)) {
//                                case"size"_psh: {
//                                    project.screenLayout.width = layoutEl.nodes[0].getReal<RealGL>();
//                                    project.screenLayout.height = layoutEl.nodes[1].getReal<RealGL>();
//                                    break; }
//                                case"orientation"_psh: {
//                                    switch (pshHash(layoutEl.getValue())) {
//                                    case"portrait"_psh:project.screenLayout.orientation = mangl::ScreenOrientation::Portrait; break;
//                                    case"landscape"_psh:project.screenLayout.orientation = mangl::ScreenOrientation::Landscape; break;
//                                    default:apl::throwError("Invalid value", layoutEl.getValue());
//                                    }
//                                    break;
//                                }//screen.screenLayout.orientation = layoutEl.getValue(); break;
//                                case"constraint"_psh: {
//                                    switch (pshHash(layoutEl.getValue())) {
//                                    case"width"_psh:project.screenLayout.constraint = mangl::ScreenConstraint::Width; break;
//                                    case"height"_psh:project.screenLayout.constraint = mangl::ScreenConstraint::Height; break;
//                                    default:
//                                        break;
//                                    }
//                                }
//                                default:
//                                    break;
//                                }
//                            }
//
//                            break;
//                        }
//                        case"screens"_psh: {
//                            Screen screen;
//                            for (auto& screenNode : projectEl) {
//                                screen.viewPort.x = 0;
//                                screen.viewPort.y = 0;
//                                screen.viewPort.width = project.screenLayout.width;
//                                screen.viewPort.height = project.screenLayout.height;
//                                for (auto& screenEl : screenNode) {
//                                    switch (pshHash(screenEl.key)) {
//                                    case"id"_psh: screen.id = screenEl.getString(); break;
//                                    case"title"_psh: screen.title = screenEl.getString(); break;
//                                    case"viewPort"_psh: {
//                                        for (auto& viewPortEl : screenEl) {
//                                            switch (pshHash(viewPortEl.key)) {
//                                            case"x"_psh: screen.viewPort.x = viewPortEl.getReal<RealGL>(); break;
//                                            case"y"_psh: screen.viewPort.y = viewPortEl.getReal<RealGL>(); break;
//                                            case"size"_psh: {
//                                                screen.viewPort.width = viewPortEl.nodes[0].getReal<RealGL>();
//                                                screen.viewPort.height = viewPortEl.nodes[1].getReal<RealGL>();
//                                                break;
//                                            }
//                                            default:
//                                                break;
//                                            }
//                                        }
//                                        break;
//                                    }
//                                    case"worlds"_psh: {
//                                        World world;
//                                        for (auto& worldNode : screenEl) {                        
//                                            world.width = project.screenLayout.width;
//                                            world.height = project.screenLayout.height;
//                                            for (auto& worldEl : worldNode) {
//                        
//                                                switch (pshHash(worldEl.key)) {
//                                                case"id"_psh: world.id = worldEl.getString(); break;
//                                                case"title"_psh: world.title = worldEl.getString(); break;
//                                                case"size"_psh: {
//                                                    world.width = worldEl.nodes[0].getReal<RealGL>();
//                                                    world.height = worldEl.nodes[1].getReal<Real>();
//                                                    break;
//                                                }                                                
//                                                case"components"_psh: {                                                
//                                                    for (auto& componentNode : worldEl) {
//                                                        Component component;
//                                                        for (auto& componentEl : componentNode) {
//                                                            switch (pshHash(componentEl.key)) {
//                                                            case"id"_psh: component.id = componentEl.getString(); break;
//                                                            case"type"_psh: component.type = componentEl.getString(); break;
//                                                            case"left"_psh: {
//                                                                auto l = parseCoords(componentEl.getString(),project.screenLayout.width);
//                                                                component.componentLayout.left = l;
//                                                                break;
//                                                            }
//                                                            case"right"_psh: {
//                                                                auto r = parseCoords(componentEl.getString(), project.screenLayout.width);
//                                                                component.componentLayout.right = r;
//                                                                break;
//                                                            }
//                                                            case"top"_psh: {
//                                                                auto top = parseCoords(componentEl.getString(), project.screenLayout.height);
//                                                                component.componentLayout.top = top;
//                                                                break;
//                                                            }
//                                                            case"bottom"_psh: {
//                                                                auto bottom = parseCoords(componentEl.getString(), project.screenLayout.height);
//                                                                component.componentLayout.bottom = bottom;
//                                                                break;
//                                                            }
//                                                            case"width"_psh: {
//                                                                auto width = parseCoords(componentEl.getString(), project.screenLayout.width);
//                                                                component.componentLayout.width = width;
//                                                                break;
//                                                            }
//                                                            case"height"_psh: {
//                                                                auto height = parseCoords(componentEl.getString(), project.screenLayout.height);
//                                                                component.componentLayout.height = height;
//                                                                break;
//                                                            }
//                                                            //case"alignHorz"_psh: component.componentLayout.alignHorz = componentEl.getInt();
//                                                            case"alignHorz"_psh: {
//                                                                parseHorzAlign(componentEl.getString(), component.componentLayout.alignHorz);
//                                                                break;
//                                                            }// component.componentLayout.alignHorz = parseHorzAlign(componentEl.getString(), component.componentLayout.alignHorz); break;
//                                                            case"alignVert"_psh: {
//                                                                parseVertAlign(componentEl.getString(), component.componentLayout.alignVert);
//                                                                break;
//                                                            } //component.componentLayout.alignVert = parseVertAlign(componentEl.getString(), component.componentLayout.alignVert); break;
//                                                            case"border"_psh: {
//                                                                for (auto& borderEl : componentEl) {
//                                                                    switch (pshHash(borderEl.key)) {
//                                                                    case"size"_psh: component.border.size = borderEl.getReal<Real>(); break;
//                                                                    case"color"_psh: component.border.color = parseColor(borderEl.nodes[0].getString()); break;
//                                                                    case"style"_psh: {
//                                                                        switch (pshHash(borderEl.getValue())) {
//                                                                        case"solid"_psh: component.border.style = mangl::BorderStyle::Solid; break;
//                                                                        default: 
//                                                                            break;
//                                                                        }
//                                                                        break;
//                                                                    }
//                                                                    
//                                                                    default :
//                                                                        break;
//                                                                    }
//                                                                }
//                                                            
//                                                                break;
//                                                            }//component.border = componentEl.getReal<RealGL>(); break;
//                                                            case"color"_psh: {
//                                                                for (auto& colorNode : componentEl) {
//                                                                    component.color.push_back(parseColor(colorNode.getString()));
//                                                                }
//                                                                break;
//                                                            }
//                                                            case"backgroundColor"_psh: {
//                                                                for (auto& backColorNode : componentEl) {
//                                                                    component.backgroundColor.push_back(parseColor(backColorNode.getString()));
//                                                                }
//                                                                break;
//                                                            }
//                                                            case"fillType"_psh: component.fillType = componentEl.getString(); break;
//                                                            case"name"_psh:{
//                                                                //fontNames.insert(componentEl.getString());
//                                                                component.name = componentEl.getString(); break;}
//                                                            //case"file"_psh: component.file = componentEl.getString(); break;
//                                                            case"text"_psh: component.text = componentEl.getString(); break;
//                                                            default:
//                                                                break;
//                                                            }
//                                                        }
//                                                        world.components.push_back(component);
//                                                    }
//                                                    break;
//                                                }
//                                                
//                                                default:
//                                                    break;
//                                                }
//                                            }
//                                        }
//                                        screen.worlds.push_back(world);
//                                        break;
//                                        
//                                    }
//                                    default:
//                                        break;
//                                    }
//                                }
//                            }
//                            project.screens.push_back(screen);
//                            break;
//                        }
//                        default:
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//
//    
//    }
//    return project;
//    
//}
//
//Rect calcCoords(const Component& elem,const Project& project,const Screen& screen ) {
//    Rect r{};
//    r.w = elem.componentLayout.width.value;
//    if (r.w == 0 && elem.componentLayout.left.value != 0 && elem.componentLayout.right.value != 0) {
//        r.w = project.screenLayout.width - elem.componentLayout.left.value - elem.componentLayout.right.value;
//    }
//    r.h = elem.componentLayout.height.value;
//    if (r.h == 0 && elem.componentLayout.top.value != 0 && elem.componentLayout.bottom.value != 0) {
//        r.h = project.screenLayout.height - elem.componentLayout.top.value - elem.componentLayout.bottom.value;
//    }
//    r.x = elem.componentLayout.left.value;
//    if (r.x == 0 && elem.componentLayout.right.value != 0) {
//        r.x = project.screenLayout.width - elem.componentLayout.right.value - r.w;
//    }
//    r.x += elem.componentLayout.alignHorz;
//    r.x += screen.viewPort.x;
//    r.y = elem.componentLayout.top.value;
//    if (r.y == 0 && elem.componentLayout.bottom.value != 0) {
//        r.y = project.screenLayout.height - elem.componentLayout.bottom.value;
//    }
//    r.y += elem.componentLayout.alignVert;
//    r.y += screen.viewPort.y;
//    return r;
//
//}
//
//Point calcPointCoords(const Component& elem, const Project& project, const Screen& screen) {
//    Point p{};
//    p.x = elem.componentLayout.left.value;
//    if (p.x == 0 && elem.componentLayout.right.value != 0) {
//        p.x = project.screenLayout.width - elem.componentLayout.right.value;
//    }
//    p.x += elem.componentLayout.alignHorz;
//    p.x += screen.viewPort.x;
//
//    p.y = elem.componentLayout.top.value;
//    if (p.y == 0 && elem.componentLayout.bottom.value != 0) {
//        p.y = project.screenLayout.height - elem.componentLayout.bottom.value;
//    }
//    p.y += elem.componentLayout.alignVert;
//    p.y += screen.viewPort.y;
//    return p;
//}
//
//
//void createScreenComponents(const Project& project, Index screenIndex,const TextureAtlas& gameTex) {
//    auto scene = App::instance()->scene();
//
//    const auto& screen = project.screens[screenIndex] ;
//    for (auto& w : screen.worlds) {
//        for (auto& elem : w.components) {
//            switch (pshHash(elem.type)) {
//            case"rectangle"_psh: {
//                auto r = calcCoords(elem, project,screen);
//                //auto c = M::White;
//                auto c = elem.color[0];
//                auto prim = scene->newRect(c, r);
//                if (elem.border.size) {
//                    prim->setWidth(elem.border.size);
//                }
//                break;
//            }
//            case"label"_psh: {
//                auto& font = gameTex.getFont(elem.name);
//                auto p = calcPointCoords(elem, project, screen);
//                auto textHeight = elem.componentLayout.height.value;
//                auto labelColor = M::Silver;
//                //Color textColor{M::White};
//                auto textColor = elem.color[0];
//            
//                scene->newLabel(elem.text, p, textHeight, textColor, M::AlignCenterAll)->setFont(&font);
//                break;
//            }
//            case"image"_psh: {
//                auto& image = gameTex.get(elem.name);
//                auto r = calcCoords(elem, project, screen);
//                scene->newImage(r,image);
//                break;
//            }
//            case"button"_psh: {
//                auto r = calcCoords(elem, project, screen);
//                auto& button = gameTex.get(elem.name);
//                scene->newButton(button,r);
//                
//                break;
//            }
//
//            default:
//                break;
//            
//            }
//        }
//    }
//
//
//}
//
//
//
//void parseHorzAlign(StrView value, int& alignHorz)
//{
//    auto s = apl::str_trim(value);
//
//    if ("left" == s || "LEFT" == s) alignHorz = M::AlignLeft; else
//        if ("center" == s || "CENTER" == s) alignHorz = M::AlignCenter; else
//            if ("right" == s || "RIGHT" == s) alignHorz = M::AlignRight;
//    //else
//    //raiseError("Invalid horizontal align value: %s", s);
//
//}
//void parseVertAlign(StrView value, int& alignVert)
//{
//    auto s = apl::str_trim(value);
//
//    if ("bottom" == s || "BOTTOM" == s) alignVert = M::AlignBottom; else
//        if ("center" == s || "middle" == s || "CENTER" == s || "MIDDLE" == s) alignVert = M::AlignVCenter; else
//            if ("top" == s || "TOP" == s) alignVert = M::AlignTop;
//    //else
//    //raiseError("Invalid vertical align value: %s", s);
//
//}
//
//
//
////void createRect(const Project& project) {
////    auto scene = App::instance()->scene();
////
////    for (auto& pr : project.screens) {
////        for (auto& w : pr.worlds) {
////            for (auto& elem : w.components) {
////                if ("rect" == elem.type) {
////                    auto r =calcCoords(elem, project,w);
////                    auto c = M::White;
////                    auto prim = scene->newRect(c, r);
////                    if (elem.border.size) {
////                        prim->setWidth(elem.border.size);
////                    }
////                }
////
////            }
////        }
////    }
////   
////}
//
////void createLabel(const Project& project) {
////    auto scene = App::instance()->scene();
////    TextureAtlas gameTex;
////    gameTex.load("C:\\Users\\Arus\\Dropbox\\apps\\pentix\\textures\\320x480\\tex_gui");
////    for (auto& pr : project.screens) {
////        for (auto& w : pr.worlds) {
////            for (auto& compNode : w.components) {
////                if ("label" == compNode.type) {
////                    
////                    auto& font = gameTex.getFont(compNode.name);
////
////                    Cord textHeight{};
////                    Color labelColor{M::Silver};
////                    Color textColor{M::White};
////
////                    scene->newLabel(compNode.text, {160, 50}, textHeight, textColor, M::AlignCenterAll)->setFont(&font);
////}
////            }
////        }
////    }
////}
//
//
//
//std::set<String> loadFontNames(const Project& project) {
//    std::set<String> fontNames;
//    for (auto& pr : project.screens) {
//        for (auto& w : pr.worlds) {
//            for (auto& el : w.components) {
//                if ("label" == el.type) {
//                    fontNames.insert(el.name);
//                    return fontNames;
//                }
//            }
//        }
//    }
//}
//
//
////String getText(const Project& project) {
////    for (auto& pr : project.screens) {
////        for (auto& w : pr.worlds) {
////            for (auto& el : w.components) {
////                if ("label" == el.type) {
////                    return el.text;
////                }
////            }
////        }
////    }
////}
//
//


}  // namespace mangl