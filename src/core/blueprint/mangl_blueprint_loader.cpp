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

void Loader::load(StrView filename, BlueprintP blueprint)
{
/*
    //auto& project = *blueprint;

    //loadDeps(filename, blueprint);

    auto yaml = loadYaml(filename);

    for (auto& doc : yaml.documents) {
        for (auto& docNode : doc) {
            if ("project" == docNode.key) {
                for (auto& projectNode : docNode) {
                    for (auto& projectEl : projectNode) {
                        switch (pshHash(projectEl.key)) {
                        case"id"_psh: blueprint->id = projectEl.getString(); break;
                        case"title"_psh: blueprint->title = projectEl.getString(); break;
                        case"screenLayout"_psh: {
                            for (auto& layoutEl : projectEl) {
                                switch (pshHash(layoutEl.key)) {
                                case"size"_psh: {
                                    blueprint->layout.width = layoutEl.nodes[0].getReal<RealGL>();
                                    blueprint->layout.height = layoutEl.nodes[1].getReal<RealGL>();
                                    break; }
                                case"orientation"_psh: {
                                    switch (pshHash(layoutEl.getValue())) {
                                    case"portrait"_psh:blueprint->layout.orientation = mangl::ScreenOrientation::Portrait; break;
                                    case"landscape"_psh:blueprint->layout.orientation = mangl::ScreenOrientation::Landscape; break;
                                    default:apl::throwError("Invalid value", layoutEl.getValue());
                                    }
                                    break;
                                }
                                case"constraint"_psh: {
                                    switch (pshHash(layoutEl.getValue())) {
                                    case"width"_psh:blueprint->layout.constraint = mangl::ScreenConstraint::Width; break;
                                    case"height"_psh:blueprint->layout.constraint = mangl::ScreenConstraint::Height; break;
                                    default:
                                        break;
                                    }
                                }
                                default:
                                    break;
                                }
                            }

                            break;
                        }
                        case"screens"_psh: {
                            loadScreen(projectEl,blueprint);
                            break;
                        }
                        default:
                            break;
                        }
                    }
                }
            }
        }
    }
*/

}

    /*

void Loader::loadScreen(YamlNodeA node, BlueprintP blueprint) {
    Screen screen;
    for (auto& screenNode : node) {
        screen.viewPort.x = 0;
        screen.viewPort.y = 0;
        screen.viewPort.width = blueprint->layout.width;
        screen.viewPort.height = blueprint->layout.height;
        for (auto& screenEl : screenNode) {
            switch (pshHash(screenEl.key)) {
            case"id"_psh: screen.id = screenEl.getString(); break;
            case"title"_psh: screen.title = screenEl.getString(); break;
            case"viewPort"_psh: {
                for (auto& viewPortEl : screenEl) {
                    switch (pshHash(viewPortEl.key)) {
                    case"x"_psh: screen.viewPort.x = viewPortEl.getReal<RealGL>(); break;
                    case"y"_psh: screen.viewPort.y = viewPortEl.getReal<RealGL>(); break;
                    case"size"_psh: {
                        screen.viewPort.width = viewPortEl.nodes[0].getReal<RealGL>();
                        screen.viewPort.height = viewPortEl.nodes[1].getReal<RealGL>();
                        break;
                    }
                    default:
                        break;
                    }
                }
                break;
            }
            case"worlds"_psh: {
                loadWorld(screenEl,screen,blueprint);
                break;
            }
            default:
                break;
            }
        }
    }
    blueprint->screens.emplace_back(screen);

}



void Loader::loadWorld(YamlNodeA node,Screen& screen, BlueprintP blueprint) {
    World world;
    for (auto& worldNode : node) {
        world.width = blueprint->layout.width;
        world.height = blueprint->layout.height;
        for (auto& worldEl : worldNode) {

            switch (pshHash(worldEl.key)) {
            case"id"_psh: world.id = worldEl.getString(); break;
            case"title"_psh: world.title = worldEl.getString(); break;
            case"size"_psh: {
                world.width = worldEl.nodes[0].getReal<RealGL>();
                world.height = worldEl.nodes[1].getReal<Real>();
                break;
            }
            case"components"_psh: {
                loadComponent(worldEl,world,blueprint);
                break;
            }
            default:
                break;
            }
        }
    }
    screen.worlds.push_back(world);
}



void Loader::loadComponent(YamlNodeA node,World& world, BlueprintP blueprint) {
    for (auto& componentNode : node) {

        Component component;
        for (auto& componentEl : componentNode) {
            switch (pshHash(componentEl.key)) {
            case"id"_psh: component.id = componentEl.getString(); break;
            case"type"_psh: component.type = componentEl.getString(); break;
            case"left"_psh: {
                auto l = parseCoords(componentEl.getString(), blueprint->layout.width);
                component.componentLayout.left = l;
                break;
            }
            case"right"_psh: {
                auto r = parseCoords(componentEl.getString(), blueprint->layout.width);
                component.componentLayout.right = r;
                break;
            }
            case"top"_psh: {
                auto top = parseCoords(componentEl.getString(), blueprint->layout.height);
                component.componentLayout.top = top;
                break;
            }
            case"bottom"_psh: {
                auto bottom = parseCoords(componentEl.getString(), blueprint->layout.height);
                component.componentLayout.bottom = bottom;
                break;
            }
            case"width"_psh: {
                auto width = parseCoords(componentEl.getString(), blueprint->layout.width);
                component.componentLayout.width = width;
                break;
            }
            case"height"_psh: {
                auto height = parseCoords(componentEl.getString(), blueprint->layout.height);
                component.componentLayout.height = height;
                break;
            }
            case"alignHorz"_psh: {
                parseHorzAlign(componentEl,componentEl.getString(), component.componentLayout.alignHorz);
                break;
            }
            case"alignVert"_psh: {
                parseVertAlign(componentEl,componentEl.getString(), component.componentLayout.alignVert);
                break;
            } 
            case"border"_psh: {
                for (auto& borderEl : componentEl) {
                    switch (pshHash(borderEl.key)) {
                    case"size"_psh: component.border.size = borderEl.getReal<Real>(); break;
                    case"color"_psh: component.border.color = parseColor(borderEl.nodes[0].getString()); break;
                    case"style"_psh: {
                        switch (pshHash(borderEl.getValue())) {
                        case"solid"_psh: component.border.style = mangl::BorderStyle::Solid; break;
                        default:
                            break;
                        }
                        break;
                    }

                    default:
                        break;
                    }
                }

                break;
            }
            case"color"_psh: {
                for (auto& colorNode : componentEl) {
                    component.color.push_back(parseColor(colorNode.getString()));
                }
                break;
            }
            case"backgroundColor"_psh: {
                for (auto& backColorNode : componentEl) {
                    component.backgroundColor.push_back(parseColor(backColorNode.getString()));
                }
                break;
            }
            case"fillType"_psh: component.fillType = componentEl.getString(); break;
            case"name"_psh: {
                component.name = componentEl.getString(); break; }
            case"text"_psh: component.text = componentEl.getString(); break;
            default:
                break;
            }
        }
        world.components.push_back(component);
    }

}

void Loader::parseHorzAlign(YamlNodeA node,StrView value, int& alignHorz)
{
    auto s = apl::str_trim(value);

    if ("left" == s || "LEFT" == s) alignHorz = M::AlignLeft; else
        if ("center" == s || "CENTER" == s) alignHorz = M::AlignCenter; else
            if ("right" == s || "RIGHT" == s) alignHorz = M::AlignRight;
    else
        raiseParseError(node,"Invalid horizontal align value: %s", s);

}
void Loader::parseVertAlign(YamlNodeA node,StrView value, int& alignVert)
{
    auto s = apl::str_trim(value);

    if ("bottom" == s || "BOTTOM" == s) alignVert = M::AlignBottom; else
        if ("center" == s || "middle" == s || "CENTER" == s || "MIDDLE" == s) alignVert = M::AlignVCenter; else
            if ("top" == s || "TOP" == s) alignVert = M::AlignTop;
    else
        raiseParseError(node,"Invalid vertical align value: %s", s);

}


CoordMajor Loader::parseCoords(String str, RealGL max) {
    CoordMajor obj{};
    if ('%' == str.back()) {
        auto val = str.substr(0, str.length() - 1);
        obj.value = (max * stoi(val)) / 100;
        obj.units = CoordUnit::Percent;
        obj.max = max;
    } else {
        obj.value = stoi(str);
        obj.max = max;
    }
    return obj;
}

Color Loader::parseColor(StrView color) {
    return apl::parseUIntHex(color);
}

std::set<String> Loader::loadFontNames(const Project& project) {
    std::set<String> fontNames;
    for (auto& pr : project.screens) {
        for (auto& w : pr.worlds) {
            for (auto& el : w.components) {
                if ("label" == el.type) {
                    fontNames.insert(el.name);
                    return fontNames;
                }
            }
        }
    }

    return fontNames;
}



void Loader::loadExpression(YamlNodeA& node, Expression& exp) {
    for (auto& expNode : node) {
        switch (pshHash(expNode.key)) {
        case"operator"_psh: {
            exp.expType = mangl::ExpType::Operator;
            for (auto& operatorNode : expNode) {
                switch (pshHash(operatorNode.key)) {
                case"type"_psh: {
                    switch (pshHash(operatorNode.getString())) {
                    case"unary_minus"_psh:exp.oper.type = mangl::OperatorType::UnaryMinus; break;
                    case"minus"_psh:exp.oper.type = mangl::OperatorType::Minus; break;
                    case"plus"_psh:exp.oper.type = mangl::OperatorType::Plus; break;
                    case"multiplication"_psh:exp.oper.type = mangl::OperatorType::Multiplication; break;
                    case"division"_psh:exp.oper.type = mangl::OperatorType::Division; break;                  
                    default:
                        break;
                    }
                    break;
                }
                case"arg1"_psh: {
                    exp.oper.arg1 = std::make_unique<Expression>();
                    auto& arg1 = *exp.oper.arg1;
                    for (auto& argEl : operatorNode) {
                        switch (pshHash(argEl.key)) {
                        case"type"_psh:{
                            switch (pshHash(argEl.getString())){
                            case"value"_psh: {
                                arg1.argType = mangl::ArgType::Value;
                                break;
                            }
                            case"variable"_psh: {
                                arg1.argType = mangl::ArgType::Variable;
                                break;
                            }
                            case"expression"_psh: {
                                arg1.argType = mangl::ArgType::Expression;
                                break;
                            }

                            default:
                                break;
                            }
                            break;
                        }
                        case"value"_psh: {
                            switch (arg1.argType) {
                            case(mangl::ArgType::Value):{
                                arg1.value = argEl.getReal<RealGL>();
                                break;
                            }
                            case(mangl::ArgType::Variable): {
                                arg1.variable = argEl.getString();
                                break;
                            }
                            case(mangl::ArgType::Expression): {
                                loadExpression(argEl, arg1);
                                break;
                            }
                            
                            default:
                                break;
                            }
                            break;
                        }
                        
                        default:
                            break;
                        }
                    
                    }
                    break;
                }
                case"arg2"_psh: {
                    exp.oper.arg2 = std::make_unique<Expression>();
                    auto& arg2 = *exp.oper.arg2;
                    for (auto& argEl : operatorNode) {
                        switch (pshHash(argEl.key)) {
                        case"type"_psh: {
                            switch (pshHash(argEl.getString())) {
                            case"value"_psh: {
                                arg2.argType = mangl::ArgType::Value;
                                break;
                            }
                            case"variable"_psh: {
                                arg2.argType = mangl::ArgType::Variable;
                                break;
                            }
                            case"expression"_psh: {
                                arg2.argType = mangl::ArgType::Expression;
                                break;
                            }

                            default:
                                break;
                            }
                            break;
                        }
                        case"value"_psh: {
                            switch (arg2.argType) {
                            case(mangl::ArgType::Value): {                                                         
                                arg2.value = argEl.getReal<RealGL>();
                                break;
                            }
                            case(mangl::ArgType::Variable): {
                                arg2.variable = argEl.getString();
                                break;
                            }
                            case(mangl::ArgType::Expression): {
                                loadExpression(argEl, arg2);
                                break;
                            }

                            default:
                                break;
                            }
                            break;
                        }

                        default:
                            break;
                        }

                    }
                    break;
                }

                default:
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
    }
 
}

void Loader::loadOp(YamlNodeA& el, Op& op) {
            switch (pshHash(el.key)) {
            case"op"_psh: op.type = el.getString(); break;
            case"condition"_psh: {
                loadExpression(el, op.condition.expression);
                break;
            }
            case"block"_psh: {
                op.block = std::make_unique<Block>();
                auto& bl = *op.block;
                for (auto& bNode : el) {
                    for (auto& bEl : bNode) {
                        auto& bop = bl.ops.emplace_back();
                        loadOp(bEl, bop);
                    }
                }
                break;
            }
            case"else"_psh: {
                op.elseBlock = std::make_unique<Block>();
                auto& bl = *op.elseBlock;
                for (auto& bNode : el) {
                    for (auto& bEl : bNode) {
                        auto& bop = bl.ops.emplace_back();
                        loadOp(bEl, bop);
                    }
                }
                break;
            }
            case"args"_psh: {
                for (auto& argsNode : el) {
                    switch (pshHash(argsNode.key)) {
                    case"id"_psh: op.args.id = argsNode.getString(); break;
                    case"by"_psh: {
                        op.args.by.x = argsNode.nodes[0].getReal<RealGL>();
                        op.args.by.y = argsNode.nodes[1].getReal<RealGL>();

                        break;
                    }
                    case"to"_psh: {
                        op.args.to.x = argsNode.nodes[0].getReal<RealGL>();
                        op.args.to.y = argsNode.nodes[1].getReal<RealGL>();
                        break;
                    }

                    default:
                        break;
                    }
                }
                break;
            }

            default:
                break;
            }

}



void Loader::loadFunction(StrView filename) {

    auto yaml = loadYaml(filename);
    Function function;

    int opIndex = 0;

    for (auto& doc : yaml.documents) {
        for (auto& docNode : doc) {
            if ("function" == docNode.key) {
                for (auto& functionNode : docNode) {
                    if ("block" == functionNode.key) {
                        auto& block = function.block;
                        for (auto& blockNode : functionNode) {
                            auto& op = block.ops.emplace_back();
                            for (auto& blockEl : blockNode) {
                                loadOp(blockEl,op);

                            }
                        }
                    }
                }
            }
        }
    }
}
*/

auto Loader::loadDeps(StrView filename, BlueprintP blueprint) -> const DepsContainer&
{
    // blueprint_ = blueprint;

    // if (mainModule_.filename != filename) {
    //     blueprint_->clear();

    //     deps_.clear();

    //     auto dep = deps_.add(filename);
    //     dep->path = filename;

    //     mainModule_ = parseModule(filename, dep);
    // }

    return deps_;
}

/*
auto Loader::parseModule(StrView filename, DepP parent) -> Module
{
    Module mod;
    mod.filename = locateModule(filename, parent);
    mod.parsedData = loadYaml(mod.filename);

    parseFileStack_.push(mod.filename);

    for (auto& doc: mod.parsedData.documents) {
        parseModuleTags(filename, doc, &mod, parent);
    }

    parseFileStack_.pop();

    return mod;
}

String Loader::locateModule(YamlNodeA yaml, StrView filename, DepP parent)
{
    if (fileExists(filename))
        return String{filename};

    verify(parent, "Cannot find: %s", filename);

    auto parentDir = extractDirName(parent->path);
    auto path = makeFilePath(parentDir, filename);
    if (fileExists(path))
        return path;

    throwError("File not found: %s", filename);
}

*/


}  // namespace mangl::blueprint
