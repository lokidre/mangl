/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
/* *** Automatically Generated *** DO NOT EDIT *** */
#pragma once

#include "state.hpp"
#include "main_types.hpp"
#include "apeal/singleton.hpp"
#include "mangl/packet.h"

#include <apeal/parse/yaml.hpp>
#include <string>
#include <string_view>

inline void pack(const ModelViewState& obj, apl::YamlBuilder* yaml);
inline void unpack(const apl::Yaml& yaml, ModelViewState* obj);


namespace impl_modelViewState_yaml {


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) {
    return false;
}

inline bool isEmptyYaml(std::string_view name) {
    return name.empty();
}
inline void packObject(const undefined& obj, std::string_view name, apl::YamlBuilder* yaml);
inline void packObject(const undefined& obj, std::integral auto name, apl::YamlBuilder* yaml);
inline void unpackObject(const apl::YamlNode& node, undefined* obj);


inline void packObject(const undefined& obj, std::string_view name, apl::YamlBuilder* yaml) {

    if (isEmptyYaml(name)) {
        yaml->map();
    } else {
        yaml->obj(name);
    }
    if (obj.ambientColor != mangl::M::Black)
        yaml->field("ambientColor", obj.ambientColor);
    if (obj.diffuseColor != mangl::M::Red)
        yaml->field("diffuseColor", obj.diffuseColor);
    if (obj.specularColor != mangl::M::Blue)
        yaml->field("specularColor", obj.specularColor);
    if (obj.shininess != 32)
        yaml->field("shininess", obj.shininess);
    if (obj.wireframe != 0)
        yaml->field("wireframe", obj.wireframe);
    if (obj.vertices != 0)
        yaml->field("vertices", obj.vertices);
    if (obj.faceCulling != 0)
        yaml->field("faceCulling", obj.faceCulling);
    if (obj.depthTest != 0)
        yaml->field("depthTest", obj.depthTest);
    if (obj.lighting != 0)
        yaml->field("lighting", obj.lighting);
    yaml->pop();

}
inline void packObject(const undefined& obj, std::integral auto name, apl::YamlBuilder* yaml) {

    if (isEmptyYaml(name)) {
        yaml->map();
    } else {
        yaml->obj(name);
    }
    if (obj.ambientColor != mangl::M::Black)
        yaml->field("ambientColor", obj.ambientColor);
    if (obj.diffuseColor != mangl::M::Red)
        yaml->field("diffuseColor", obj.diffuseColor);
    if (obj.specularColor != mangl::M::Blue)
        yaml->field("specularColor", obj.specularColor);
    if (obj.shininess != 32)
        yaml->field("shininess", obj.shininess);
    if (obj.wireframe != 0)
        yaml->field("wireframe", obj.wireframe);
    if (obj.vertices != 0)
        yaml->field("vertices", obj.vertices);
    if (obj.faceCulling != 0)
        yaml->field("faceCulling", obj.faceCulling);
    if (obj.depthTest != 0)
        yaml->field("depthTest", obj.depthTest);
    if (obj.lighting != 0)
        yaml->field("lighting", obj.lighting);
    yaml->pop();

}
inline void unpackObject(const apl::YamlNode& node, undefined* obj){

    for (auto& objNode: node){
        if ("ambientColor" == objNode.key) {
            objNode.get(&(obj->ambientColor));
            continue;
        }
        if ("diffuseColor" == objNode.key) {
            objNode.get(&(obj->diffuseColor));
            continue;
        }
        if ("specularColor" == objNode.key) {
            objNode.get(&(obj->specularColor));
            continue;
        }
        if ("shininess" == objNode.key) {
            objNode.get(&(obj->shininess));
            continue;
        }
        if ("wireframe" == objNode.key) {
            objNode.get(&(obj->wireframe));
            continue;
        }
        if ("vertices" == objNode.key) {
            objNode.get(&(obj->vertices));
            continue;
        }
        if ("faceCulling" == objNode.key) {
            objNode.get(&(obj->faceCulling));
            continue;
        }
        if ("depthTest" == objNode.key) {
            objNode.get(&(obj->depthTest));
            continue;
        }
        if ("lighting" == objNode.key) {
            objNode.get(&(obj->lighting));
            continue;
        }
    }

}

}  // namespace impl_modelViewState_yaml


inline std::string packString(const ModelViewState& obj){
    apl::YamlBuilder yaml;
    pack(obj, &yaml);
    return yaml.str();
}

inline void packFile(const ModelViewState& obj, std::string_view fileName){
    auto strYaml = packString(obj);
    apl::saveFile(fileName,strYaml);
}

inline void unpack(const apl::Yaml& yaml, ModelViewState* obj){
    for (auto& doc : yaml.documents) {
         for (auto& docNode: doc) {
             impl_modelViewState_yaml::unpackObject(docNode, obj );
         }
   }
}

inline void unpackYaml(std::string_view text, ModelViewState* obj){ 
    auto yaml = apl::parseYaml(text);
    unpack(yaml, obj);
}



inline void pack(const ModelViewState& obj, apl::YamlBuilder* yaml){
    yaml->begin();
    impl_modelViewState_yaml::packObject(obj, "modelViewState", yaml);
    yaml->end();
}

