/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
//#include "objects_parse_yaml.h"
#include <mangl/mangl.h>
#include <apeal/parse/ini.hpp>
#include <apeal/parse/yaml.hpp>
#include <apeal/file.hpp>
#include <apeal/hash.hpp>

#include "../../../src/core/blueprint/mangl_blueprint_loader.h"

namespace mangl {

using namespace apl;


//void loadFunction(StrView filename) {
//
//    auto yaml = loadYaml(filename);
//    Function function;
//
//    int opIndex = 0;
//
//    for (auto& doc : yaml.documents) {
//        for (auto& docNode : doc) {
//            if ("function" == docNode.key) {
//                for (auto& functionNode : docNode) {
//                    if ("block" == functionNode.key) {
//                        //Block block;
//                        auto& block = function.block;
//
//                        for (auto& blockNode : functionNode) {
//                            for (auto& blockEl : blockNode) {
//                                auto& op = block.ops.emplace_back();
//                                //auto& pop = block.ops.emplace_back(std::make_unique<Op>());
//                                //auto& op = *pop;
//                                //auto pop = std::make_unique<Op>();
//                                //auto& op = *pop;
//                                //auto& opIt = block.ops.emplace(opIndex++); // , std::make_unique<Op>());
//                                //auto& op = opIt.second;
//
//                                switch (pshHash(blockEl.key)) {                                   
//                                case"op"_psh: op.type = blockEl.getString(); break;
//                                case"condition"_psh: {
//                                    for (auto& condNode : blockEl) {
//                                        switch (pshHash(condNode.key)) {
//                                        case"expression"_psh: op.condition.expression = condNode.getString();                                       
//                                        default: 
//                                            break;
//                                        }
//                                    }
//                                    break;
//                                }
//                                                   //TODO: recursive structs
//                                case"block"_psh: {
//                                 /*   auto& bl = op.block;
//                                    for (auto& bNode : blockEl) {
//                                        for(auto)
//                                    }*/
//                                    break;
//                                }
//                                //case"else"_psh:
//                                case"args"_psh: {
//                                    for (auto& argsNode : blockEl) {
//                                        switch (pshHash(argsNode.key)) {
//                                        case"id"_psh: op.args.id = argsNode.getString(); break;
//                                        case"by"_psh: {
//                                            op.args.by.x = argsNode.nodes[0].getReal<RealGL>();
//                                            op.args.by.y = argsNode.nodes[1].getReal<RealGL>();
//
//                                            break; 
//                                        }
//                                        case"to"_psh: {
//                                            op.args.to.x = argsNode.nodes[0].getReal<RealGL>();
//                                            op.args.to.y = argsNode.nodes[1].getReal<RealGL>();
//                                            break;
//                                        }
//
//                                        default:
//                                            break;
//                                        }
//                                    }
//                                    break;
//                                }
//
//                                default:
//                                    break;
//                                }
//
//                                //block.ops.emplace(opIndex++, std::move(pop));
//                                //block.ops.push_back(op);
//                            }
//                        }
//                        //function.block = block;
//                    }
//                }
//            }
//        }
//    }
//}


}