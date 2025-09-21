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

#include "flat_object.h"

#include <apeal/index.hpp>
#include <apeal/parse/yaml.hpp>
#include <string>
#include <string_view>
#include <apeal/file.hpp>

inline void pack(const FlatObject& obj, apl::YamlBuilder* builder);
inline void unpack(const apl::Yaml& packet, FlatObject* obj);

inline auto packStringYaml(const FlatObject& obj) {
    apl::YamlBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringYaml(std::string_view str, FlatObject* obj) {
    apl::Yaml packet;
    apl::YamlParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto packFileYaml(std::string_view filename, const FlatObject& obj) {
    auto str = packStringYaml(obj);
    apl::saveFile(filename, str);
}

inline auto unpackFileYaml(std::string_view filename, FlatObject* obj) {
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}



namespace impl_FlatObject_yaml {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(FlatObject* obj);
inline bool isEmptyObject(const FlatObject& obj);


inline void clearObject(FlatObject* obj) {
    obj->intVal = {};
    obj->uintVal = {};
    obj->longVal = {};
    obj->ulongVal = {};
    obj->shortVal = {};
    obj->ushortVal = {};
    obj->doubleVal = {};
    obj->floatVal = {};
    obj->charVal = 0;
    obj->ucharVal = 0;
    obj->stringVal.clear();
    obj->charEnum = {};
    obj->intEnum = {};
    obj->vectorVal.clear();
    obj->mapVal.clear();
    obj->unorderedMapVal.clear();
    obj->setVal.clear();
}
inline bool isEmptyObject(const FlatObject& obj) {
    if (obj.intVal != 0) return false;
    if (obj.uintVal != 0) return false;
    if (obj.longVal != 0) return false;
    if (obj.ulongVal != 0) return false;
    if (obj.shortVal != 0) return false;
    if (obj.ushortVal != 0) return false;
    if (obj.doubleVal != 0) return false;
    if (obj.floatVal != 0) return false;
    if (obj.charVal != 0) return false;
    if (obj.ucharVal != 0) return false;
    if (!obj.stringVal.empty()) return false;
    if (int(obj.charEnum) != int(0)) return false;
    if (int(obj.intEnum) != int(0)) return false;
    if (!obj.vectorVal.empty()) return false;
    if (!obj.mapVal.empty()) return false;
    if (!obj.unorderedMapVal.empty()) return false;
    if (!obj.setVal.empty()) return false;
    return true;
}


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) { return false; }

inline bool isEmptyYaml(std::string_view name) { return name.empty(); }

inline void packObject(const FlatObject& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, FlatObject* obj);


inline void packObject(const FlatObject& obj, std::string_view name, apl::YamlBuilder* builder) {

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
    if (obj.doubleVal != 0)
        builder->field("doubleVal", obj.doubleVal);
    if (obj.floatVal != 0)
        builder->field("floatVal", obj.floatVal);
    if (obj.charVal != 0)
        builder->field("charVal", obj.charVal);
    if (obj.ucharVal != 0)
        builder->field("ucharVal", obj.ucharVal);
    if (!obj.stringVal.empty())
        builder->field("stringVal", obj.stringVal);
    if (static_cast<char>(obj.charEnum) != 0)
        builder->field("charEnum", static_cast<char>(obj.charEnum));
    if (static_cast<int>(obj.intEnum) != 0)
        builder->field("intEnum", static_cast<int>(obj.intEnum));
    if (!obj.vectorVal.empty()) {
        builder->flowSeqVec("vectorVal", obj.vectorVal);
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
    }

    if (!obj.unorderedMapVal.empty()) {
        builder->array("unorderedMapVal");
        for (auto& [key, val]: obj.unorderedMapVal) {
            builder->map();
            builder->field("key", key);
            builder->field("val", val);
            builder->pop();
        }
        builder->pop();  // unorderedMapVal
    }

    if (!obj.setVal.empty()) {
        builder->flowSeqVec("setVal", obj.setVal);
    }

    builder->pop();  // FlatObject

}

inline void unpackObject(const apl::YamlNode& node, FlatObject* obj) {

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

        if ("doubleVal" == objNode.key) {
            objNode.get(&obj->doubleVal);
            continue;
        }

        if ("floatVal" == objNode.key) {
            objNode.get(&obj->floatVal);
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

        if ("stringVal" == objNode.key) {
            objNode.get(&obj->stringVal);
            continue;
        }

        if ("charEnum" == objNode.key) {
            char val;
            objNode.get(&val);
            obj->charEnum = static_cast<SampleCharEnum>(val);
            continue;
        }

        if ("intEnum" == objNode.key) {
            int val;
            objNode.get(&val);
            obj->intEnum = static_cast<SampleIntEnum>(val);
            continue;
        }

        if ("vectorVal" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->vectorVal.resize(size);
            for (Index i = 0; i < size; ++i) {
                objNode.nodes[i].get(&obj->vectorVal[i]);
            }
            continue;
        }

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
            }  // for nodes
            continue;
        }

        if ("unorderedMapVal" == objNode.key) {
            for(auto& mapNode: objNode) {
                decltype(obj->unorderedMapVal)::key_type key{};
                decltype(obj->unorderedMapVal)::value_type::second_type val{};

                for(auto& valNode: mapNode) {
                    if ("key" == valNode.key) valNode.get(&key);
                    if ("val" == valNode.key) {
                        valNode.get(&val);
                    }
                }
                obj->unorderedMapVal.emplace(key, std::move(val));
            }  // for nodes
            continue;
        }

        if ("setVal" == objNode.key) {
            auto size = ssize(objNode.nodes);
            for (Index i = 0; i < size; ++i) {
                decltype(obj->setVal)::value_type val{};
                objNode.nodes[i].get(&val);
                obj->setVal.emplace(std::move(val));
            }
            continue;
        }

    }  // for nodes

}

}  // namespace impl_FlatObject_yaml


inline void pack(const FlatObject& obj, apl::YamlBuilder* builder) {
    builder->begin();
    impl_FlatObject_yaml::packObject(obj, "FlatObject", builder);
    builder->end();
}

inline void unpack(const apl::Yaml& root, FlatObject* obj) {
    impl_FlatObject_yaml::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_FlatObject_yaml::unpackObject(docNode, obj);
    }
}

