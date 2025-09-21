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

#include "nested_object.h"

#include <apeal/index.hpp>
#include <apeal/parse/yaml.hpp>
#include <string>
#include <string_view>
#include <apeal/file.hpp>

inline void pack(const NestedObject& obj, apl::YamlBuilder* builder);
inline void unpack(const apl::Yaml& packet, NestedObject* obj);

inline auto packStringYaml(const NestedObject& obj) {
    apl::YamlBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringYaml(std::string_view str, NestedObject* obj) {
    apl::Yaml packet;
    apl::YamlParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto packFileYaml(std::string_view filename, const NestedObject& obj) {
    auto str = packStringYaml(obj);
    apl::saveFile(filename, str);
}

inline auto unpackFileYaml(std::string_view filename, NestedObject* obj) {
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}



namespace impl_NestedObject_yaml {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(NestedObject* obj);
inline bool isEmptyObject(const NestedObject& obj);
inline void clearObject(SubObject* obj);
inline bool isEmptyObject(const SubObject& obj);
inline void clearObject(SubSubObject* obj);
inline bool isEmptyObject(const SubSubObject& obj);


inline void clearObject(NestedObject* obj) {
    obj->charVal = 0;
    obj->ucharVal = 0;
    obj->intVal = 0;
    obj->uintVal = 0;
    obj->floatVal = 0;
    obj->doubleVal = 0;
    clearObject(&(obj->subVal));
}
inline bool isEmptyObject(const NestedObject& obj) {
    if (obj.charVal != 0) return false;
    if (obj.ucharVal != 0) return false;
    if (obj.intVal != 0) return false;
    if (obj.uintVal != 0) return false;
    if (obj.floatVal != 0) return false;
    if (obj.doubleVal != 0) return false;
    if (!isEmptyObject(obj.subVal)) return false;
    return true;
}

inline void clearObject(SubObject* obj) {
    obj->vectorVal.clear();
    clearObject(&(obj->subSubVal));
}
inline bool isEmptyObject(const SubObject& obj) {
    if (!obj.vectorVal.empty()) return false;
    if (!isEmptyObject(obj.subSubVal)) return false;
    return true;
}

inline void clearObject(SubSubObject* obj) {
    obj->mapVal.clear();
}
inline bool isEmptyObject(const SubSubObject& obj) {
    if (!obj.mapVal.empty()) return false;
    return true;
}


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) { return false; }

inline bool isEmptyYaml(std::string_view name) { return name.empty(); }

inline void packObject(const NestedObject& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, NestedObject* obj);

inline void packObject(const SubObject& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, SubObject* obj);

inline void packObject(const SubSubObject& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, SubSubObject* obj);


inline void packObject(const NestedObject& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (obj.charVal != 0)
        builder->field("charVal", obj.charVal);
    if (obj.ucharVal != 0)
        builder->field("ucharVal", obj.ucharVal);
    if (obj.intVal != 0)
        builder->field("intVal", obj.intVal);
    if (obj.uintVal != 0)
        builder->field("uintVal", obj.uintVal);
    if (obj.floatVal != 0)
        builder->field("floatVal", obj.floatVal);
    if (obj.doubleVal != 0)
        builder->field("doubleVal", obj.doubleVal);
    if (!isEmptyObject(obj.subVal))
        packObject(obj.subVal, "subVal", builder);
    builder->pop();  // NestedObject

}

inline void unpackObject(const apl::YamlNode& node, NestedObject* obj) {

    for (auto& objNode: node) {
        if ("charVal" == objNode.key) {
            objNode.get(&obj->charVal);
            continue;
        }

        if ("ucharVal" == objNode.key) {
            objNode.get(&obj->ucharVal);
            continue;
        }

        if ("intVal" == objNode.key) {
            objNode.get(&obj->intVal);
            continue;
        }

        if ("uintVal" == objNode.key) {
            objNode.get(&obj->uintVal);
            continue;
        }

        if ("floatVal" == objNode.key) {
            objNode.get(&obj->floatVal);
            continue;
        }

        if ("doubleVal" == objNode.key) {
            objNode.get(&obj->doubleVal);
            continue;
        }

        if ("subVal" == objNode.key) {
            unpackObject(objNode, &obj->subVal);
            continue;
        }

    }  // for nodes

}

inline void packObject(const SubObject& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!obj.vectorVal.empty()) {
        builder->flowSeq("vectorVal", obj.vectorVal);
    }

    if (!isEmptyObject(obj.subSubVal))
        packObject(obj.subSubVal, "subSubVal", builder);
    builder->pop();  // SubObject

}

inline void unpackObject(const apl::YamlNode& node, SubObject* obj) {

    for (auto& objNode: node) {
        if ("vectorVal" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->vectorVal.resize(size);
            for (Index i = 0; i < size; ++i) {
                objNode.nodes[i].get(&obj->vectorVal[i]);
            }  // for nodes
            continue;
        }

        if ("subSubVal" == objNode.key) {
            unpackObject(objNode, &obj->subSubVal);
            continue;
        }

    }  // for nodes

}

inline void packObject(const SubSubObject& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!obj.mapVal.empty()) {
        builder->array("mapVal");
        for (auto& [key, val]: obj.mapVal) {
            builder->map();
            builder->field("key", key);
            builder->field("val", val);
            builder->pop();
        }
        builder->pop();  // mapVal
    }  // if (!obj.mapVal.empty())

    builder->pop();  // SubSubObject

}

inline void unpackObject(const apl::YamlNode& node, SubSubObject* obj) {

    for (auto& objNode: node) {
        if ("mapVal" == objNode.key) {
            for(auto& mapNode: objNode) {
                decltype(obj->mapVal)::key_type key{};
                decltype(obj->mapVal)::value_type::second_type val{};
                for(auto& valNode: mapNode) {
                    if ("key" == valNode.key) valNode.get(&key);
                    if ("val" == valNode.key) {
                        valNode.get(&val);
                    }
                }
                obj->mapVal.emplace(key, std::move(val));
            }
            continue;
        }

    }  // for nodes

}

}  // namespace impl_NestedObject_yaml


inline void pack(const NestedObject& obj, apl::YamlBuilder* builder) {
    builder->begin();
    impl_NestedObject_yaml::packObject(obj, "NestedObject", builder);
    builder->end();
}

inline void unpack(const apl::Yaml& root, NestedObject* obj) {
    impl_NestedObject_yaml::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_NestedObject_yaml::unpackObject(docNode, obj);
    }
}

