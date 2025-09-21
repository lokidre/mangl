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
using namespace apl;
void Engine::start(Timestamp when)
{
    startScheduled_ = true;
    startScheduleTime_ = when;
}

void Engine::reset()
{
    // TODO: remove from scene
    loader_.clear();
    blueprint_.clear();
}

/*
Rect Engine::calcCoords(const Component& elem, const Screen& screen) {
    //auto& project = *blueprint_;
    Rect r{};
    r.w = elem.componentLayout.width.value;
    if (r.w == 0 && elem.componentLayout.left.value != 0 && elem.componentLayout.right.value != 0) {
        r.w = blueprint_.layout.width - elem.componentLayout.left.value - elem.componentLayout.right.value;
    }
    r.h = elem.componentLayout.height.value;
    if (r.h == 0 && elem.componentLayout.top.value != 0 && elem.componentLayout.bottom.value != 0) {
        r.h = blueprint_.layout.height - elem.componentLayout.top.value - elem.componentLayout.bottom.value;
    }
    r.x = elem.componentLayout.left.value;
    if (r.x == 0 && elem.componentLayout.right.value != 0) {
        r.x = blueprint_.layout.width - elem.componentLayout.right.value - r.w;
    }
    r.x += elem.componentLayout.alignHorz;
    r.x += screen.viewPort.x;
    r.y = elem.componentLayout.top.value;
    if (r.y == 0 && elem.componentLayout.bottom.value != 0) {
        r.y = blueprint_.layout.height - elem.componentLayout.bottom.value;
    }
    r.y += elem.componentLayout.alignVert;
    r.y += screen.viewPort.y;
    return r;

}

Point Engine::calcPointCoords(const Component& elem, const Screen& screen) {
    //auto& project = *blueprint_;
    Point p{};
    p.x = elem.componentLayout.left.value;
    if (p.x == 0 && elem.componentLayout.right.value != 0) {
        p.x = blueprint_.layout.width - elem.componentLayout.right.value;
    }
    p.x += elem.componentLayout.alignHorz;
    p.x += screen.viewPort.x;

    p.y = elem.componentLayout.top.value;
    if (p.y == 0 && elem.componentLayout.bottom.value != 0) {
        p.y = blueprint_.layout.height - elem.componentLayout.bottom.value;
    }
    p.y += elem.componentLayout.alignVert;
    p.y += screen.viewPort.y;
    return p;
}



RealGL Engine::calculateValue(Expression& exp) {
    switch (exp.oper.type) {
    case(mangl::OperatorType::UnaryMinus): {
        return (-exp.oper.arg1->value);
    }
    case(mangl::OperatorType::Minus): {
        return (exp.oper.arg1->value - exp.oper.arg2->value);
    }
    case(mangl::OperatorType::Plus): {
        return(exp.oper.arg1->value + exp.oper.arg2->value);
    }
    case(mangl::OperatorType::Multiplication): {
        return(exp.oper.arg1->value * exp.oper.arg2->value);
    }
    case(mangl::OperatorType::Division): {
        return(exp.oper.arg1->value / exp.oper.arg2->value);
    }

    default:
        break; 
    }

    return 0;
}

RealGL Engine::parseValue(Expression& exp) {
    if (mangl::ArgType::Variable == exp.argType) {
        auto var = exp.variable;
        auto id = var.substr(0, var.find("."));
        auto property = var.substr(var.find(".") + 1, var.length());
        for (auto& screenNode : blueprint_.screens) {
            for (auto& worldeNode : screenNode.worlds) {
                for (auto& componentNode : worldeNode.components) {
                    if (id == componentNode.id) {
                        switch (pshHash(var)) {
                        case"left"_psh: {
                            return(componentNode.componentLayout.left.value);
                        }
                        case"right"_psh: {
                            return( componentNode.componentLayout.right.value);
                        }
                        case"top"_psh: {
                            return( componentNode.componentLayout.top.value);
                        }
                        case"bottom"_psh: {
                            return(componentNode.componentLayout.bottom.value);
                        }
                        case"width"_psh: {
                            return (componentNode.componentLayout.width.value);
                        }
                        case"height"_psh: {
                            return( componentNode.componentLayout.height.value);
                        }
                        default:
                            break;
                        }
                    }
                }
            }

        }
    }
    return exp.value;



}

*/

/*
RealGL Engine::evaluateExpression(Expression& exp) {

    auto& arg1 = *exp.oper.arg1;
    auto& arg2 = *exp.oper.arg2;
    exp.oper.arg1->value = parseValue(arg1);
    exp.oper.arg2->value = parseValue(arg2);
    return calculateValue(exp);
}
 */

/*
void Engine::moveComponent(const Args& args) {
    for (auto& screenNode : blueprint_.screens) {
        for (auto& worldNode : screenNode.worlds) {
            for (auto& componentNode : worldNode.components) {
                if (componentNode.id == args.id) {
                        componentNode.componentLayout.x += args.by.x;
                        componentNode.componentLayout.y += args.by.y;
                    if (args.to.x && args.to.y) {
                        componentNode.componentLayout.x = args.to.x;
                        componentNode.componentLayout.y = args.to.y;
                    }
                }
            
            }
        
        }
    
    }
}
*/

}  // namespace mangl::blueprint


