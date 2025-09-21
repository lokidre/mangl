/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
 /*** Automatically Generated *** DO NOT EDIT ***
 * Generated Date: 2024-06-20
 */
#pragma once

#include "pod_object.h"

#include <apeal/index.hpp>
#include <apeal/parse/yaml.hpp>
#include <string>
#include <string_view>
#include <apeal/file.hpp>

inline void pack(const PodObject& obj, apl::YamlBuilder* builder);
inline void unpack(const apl::Yaml& packet, PodObject* obj);

inline auto packStringYaml(const PodObject& obj) {
    apl::YamlBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringYaml(std::string_view str, PodObject* obj) {
    apl::Yaml packet;
    apl::YamlParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto packFileYaml(std::string_view filename, const PodObject& obj) {
    auto str = packStringYaml(obj);
    apl::saveFile(filename, str);
}

inline auto unpackFileYaml(std::string_view filename, PodObject* obj) {
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}



namespace impl_PodObject_yaml {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(PodObject* obj);
inline bool isEmptyObject(const PodObject& obj);


inline void clearObject(PodObject* obj) {
    obj->intVal = {};
    obj->uintVal = {};
    obj->longVal = {};
    obj->ulongVal = {};
    obj->shortVal = {};
    obj->ushortVal = {};
    obj->charVal = 0;
    obj->ucharVal = 0;
    obj->doubleVal = {};
    obj->floatVal = {};
}
inline bool isEmptyObject(const PodObject& obj) {
    if (obj.intVal != 0) return false;
    if (obj.uintVal != 0) return false;
    if (obj.longVal != 0) return false;
    if (obj.ulongVal != 0) return false;
    if (obj.shortVal != 0) return false;
    if (obj.ushortVal != 0) return false;
    if (obj.charVal != 0) return false;
    if (obj.ucharVal != 0) return false;
    if (obj.doubleVal != 0) return false;
    if (obj.floatVal != 0) return false;
    return true;
}


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) { return false; }

inline bool isEmptyYaml(std::string_view name) { return name.empty(); }

inline void packObject(const PodObject& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, PodObject* obj);


inline void packObject(const PodObject& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (obj.intVal != 0)
        builder->field("intVal", obj.intVal);
    if (obj.uintVal != 0)
        builder->field("uintVal", obj.uintVal);
    if (obj.longVal != 0)
        builder->field("longVal", obj.longVal);
    if (obj.ulongVal != 0)
        builder->field("ulongVal", obj.ulongVal);
    if (obj.shortVal != 0)
        builder->field("shortVal", obj.shortVal);
    if (obj.ushortVal != 0)
        builder->field("ushortVal", obj.ushortVal);
    if (obj.charVal != 0)
        builder->field("charVal", obj.charVal);
    if (obj.ucharVal != 0)
        builder->field("ucharVal", obj.ucharVal);
    if (obj.doubleVal != 0)
        builder->field("doubleVal", obj.doubleVal);
    if (obj.floatVal != 0)
        builder->field("floatVal", obj.floatVal);
    builder->pop();  // PodObject

}

inline void unpackObject(const apl::YamlNode& node, PodObject* obj) {

    for (auto& objNode: node) {
        if ("intVal" == objNode.key) {
            objNode.get(&obj->intVal);
            continue;
        }

        if ("uintVal" == objNode.key) {
            objNode.get(&obj->uintVal);
            continue;
        }

        if ("longVal" == objNode.key) {
            objNode.get(&obj->longVal);
            continue;
        }

        if ("ulongVal" == objNode.key) {
            objNode.get(&obj->ulongVal);
            continue;
        }

        if ("shortVal" == objNode.key) {
            objNode.get(&obj->shortVal);
            continue;
        }

        if ("ushortVal" == objNode.key) {
            objNode.get(&obj->ushortVal);
            continue;
        }

        if ("charVal" == objNode.key) {
            objNode.get(&obj->charVal);
            continue;
        }

        if ("ucharVal" == objNode.key) {
            objNode.get(&obj->ucharVal);
            continue;
        }

        if ("doubleVal" == objNode.key) {
            objNode.get(&obj->doubleVal);
            continue;
        }

        if ("floatVal" == objNode.key) {
            objNode.get(&obj->floatVal);
            continue;
        }

    }  // for nodes

}

}  // namespace impl_PodObject_yaml


inline void pack(const PodObject& obj, apl::YamlBuilder* builder) {
    builder->begin();
    impl_PodObject_yaml::packObject(obj, "PodObject", builder);
    builder->end();
}

inline void unpack(const apl::Yaml& root, PodObject* obj) {
    impl_PodObject_yaml::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_PodObject_yaml::unpackObject(docNode, obj);
    }
}

