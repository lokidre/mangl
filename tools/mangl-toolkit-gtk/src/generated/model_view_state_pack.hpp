/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
 /*** Automatically Generated *** DO NOT EDIT ***
 * Generated Date: 2024-11-15
 */
#pragma once

#include "state.hpp"
#include "main_types.hpp"
#include "apeal/singleton.hpp"
#include "mangl/packet.h"

#include <apeal/data_param.hpp>
#include <apeal/index.hpp>
#include <apeal/parse/yaml.hpp>
#include <mangl/packet.h>
#include <string>
#include <string_view>
#include <apeal/file.hpp>

namespace toolkit {

inline void pack(const ModelViewState& obj, apl::YamlBuilder* builder);
inline void unpack(const apl::Yaml& packet, ModelViewState* obj);

inline auto packStringYaml(const ModelViewState& obj) {
    apl::YamlBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringYaml(std::string_view str, ModelViewState* obj) {
    apl::Yaml packet;
    apl::YamlParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto unpackDataYaml(typename apl::DataParam<>::Arg data, ModelViewState* obj) {
    unpackStringYaml({(const char *)data.data(), data.size()}, obj);
}

inline auto packFileYaml(std::string_view filename, const ModelViewState& obj) {
    auto str = packStringYaml(obj);
    apl::saveFile(filename, str);
}

inline auto unpackFileYaml(std::string_view filename, ModelViewState* obj) {
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}

inline auto unpackFileIfExistsYaml(std::string_view filename, ModelViewState* obj) {
        if (!apl::fileExists(filename)) return;
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}



namespace impl_ModelViewState_yaml {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(ModelViewState* obj);
inline bool isEmptyObject(const ModelViewState& obj);


inline void clearObject(ModelViewState* obj) {
    Packet::clearProp(&(obj->ambientColor), mangl::M::Black);
    Packet::clearProp(&(obj->diffuseColor), mangl::M::Red);
    Packet::clearProp(&(obj->specularColor), mangl::M::Blue);
    Packet::clearProp(&(obj->shininess), 32);
    Packet::clearProp(&(obj->wireframe));
    Packet::clearProp(&(obj->vertices));
    Packet::clearProp(&(obj->faceCulling));
    Packet::clearProp(&(obj->depthTest));
    Packet::clearProp(&(obj->lighting));
}
inline bool isEmptyObject(const ModelViewState& obj) {
    if (!Packet::isPropEmpty(obj.ambientColor)) return false;
    if (!Packet::isPropEmpty(obj.diffuseColor)) return false;
    if (!Packet::isPropEmpty(obj.specularColor)) return false;
    if (!Packet::isPropEmpty(obj.shininess)) return false;
    if (!Packet::isPropEmpty(obj.wireframe)) return false;
    if (!Packet::isPropEmpty(obj.vertices)) return false;
    if (!Packet::isPropEmpty(obj.faceCulling)) return false;
    if (!Packet::isPropEmpty(obj.depthTest)) return false;
    if (!Packet::isPropEmpty(obj.lighting)) return false;
    return true;
}


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) { return false; }

inline bool isEmptyYaml(std::string_view name) { return name.empty(); }

inline void packObject(const ModelViewState& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ModelViewState* obj);


inline void packObject(const ModelViewState& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropDefault(obj.ambientColor, mangl::M::Black))
        builder->field("ambientColor", obj.ambientColor);
    if (!Packet::isPropDefault(obj.diffuseColor, mangl::M::Red))
        builder->field("diffuseColor", obj.diffuseColor);
    if (!Packet::isPropDefault(obj.specularColor, mangl::M::Blue))
        builder->field("specularColor", obj.specularColor);
    if (!Packet::isPropDefault(obj.shininess, 32))
        builder->field("shininess", obj.shininess);
    if (!Packet::isPropEmpty(obj.wireframe))
        builder->field("wireframe", obj.wireframe);
    if (!Packet::isPropEmpty(obj.vertices))
        builder->field("vertices", obj.vertices);
    if (!Packet::isPropEmpty(obj.faceCulling))
        builder->field("faceCulling", obj.faceCulling);
    if (!Packet::isPropEmpty(obj.depthTest))
        builder->field("depthTest", obj.depthTest);
    if (!Packet::isPropEmpty(obj.lighting))
        builder->field("lighting", obj.lighting);
    builder->pop();  // ModelViewState

}

inline void unpackObject(const apl::YamlNode& node, ModelViewState* obj) {

    for (auto& objNode: node) {
        if ("ambientColor" == objNode.key) {
            objNode.get(&obj->ambientColor);
            continue;
        }

        if ("diffuseColor" == objNode.key) {
            objNode.get(&obj->diffuseColor);
            continue;
        }

        if ("specularColor" == objNode.key) {
            objNode.get(&obj->specularColor);
            continue;
        }

        if ("shininess" == objNode.key) {
            objNode.get(&obj->shininess);
            continue;
        }

        if ("wireframe" == objNode.key) {
            objNode.get(&obj->wireframe);
            continue;
        }

        if ("vertices" == objNode.key) {
            objNode.get(&obj->vertices);
            continue;
        }

        if ("faceCulling" == objNode.key) {
            objNode.get(&obj->faceCulling);
            continue;
        }

        if ("depthTest" == objNode.key) {
            objNode.get(&obj->depthTest);
            continue;
        }

        if ("lighting" == objNode.key) {
            objNode.get(&obj->lighting);
            continue;
        }

    }  // for nodes

}

}  // namespace impl_ModelViewState_yaml


inline void pack(const ModelViewState& obj, apl::YamlBuilder* builder) {
    builder->begin();
    impl_ModelViewState_yaml::packObject(obj, "ModelViewState", builder);
    builder->end();
}

inline void unpack(const apl::Yaml& root, ModelViewState* obj) {
    impl_ModelViewState_yaml::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_ModelViewState_yaml::unpackObject(docNode, obj);
    }
}

}  // namespace toolkit

