/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
 /*** Automatically Generated *** DO NOT EDIT ***
 * Generated Date: 2024-09-28
 */
#pragma once

#include "../container_object.h"

#include <apeal/data_param.hpp>
#include <apeal/index.hpp>
#include <apeal/parse/yaml.hpp>
#include <mangl/packet.h>
#include <string>
#include <string_view>

namespace container_test {

inline void pack(const ContainerObject& obj, apl::YamlBuilder* builder);
inline void unpack(const apl::Yaml& packet, ContainerObject* obj);

inline auto packStringYaml(const ContainerObject& obj) {
    apl::YamlBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringYaml(std::string_view str, ContainerObject* obj) {
    apl::Yaml packet;
    apl::YamlParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto unpackDataYaml(typename apl::DataParam<>::Arg data, ContainerObject* obj) {
    unpackStringYaml({(const char *)data.data(), data.size()}, obj);
}



namespace impl_ContainerObject_yaml {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(ContainerObject* obj);
inline bool isEmptyObject(const ContainerObject& obj);
inline void clearObject(ContainerPodValue* obj);
inline bool isEmptyObject(const ContainerPodValue& obj);
inline void clearObject(ContainerVectorValue* obj);
inline bool isEmptyObject(const ContainerVectorValue& obj);
inline void clearObject(ContainerComplexValue* obj);
inline bool isEmptyObject(const ContainerComplexValue& obj);
inline void clearObject(ContainerVectorObjectValue* obj);
inline bool isEmptyObject(const ContainerVectorObjectValue& obj);

inline void clearArray(ContainerPodValue* begin, const ContainerPodValue* end);
inline bool isEmptyArray(const ContainerPodValue* begin, const ContainerPodValue* end);

inline void clearArray(std::integral auto* begin, const std::integral auto* end) {
    for (auto it = begin; it != end; ++it) *it = 0;
}
inline void clearArray(std::floating_point auto* begin, const std::floating_point auto* end) {
    for (auto it = begin; it != end; ++it) *it = 0;
}
inline bool isEmptyArray(const std::integral auto* begin, const std::integral auto* end) {
    for (auto it = begin; it != end; ++it) if (*it != 0) return false;
    return true;
}
inline bool isEmptyArray(const std::floating_point auto* begin, const std::floating_point auto* end) {
    for (auto it = begin; it != end; ++it) if (*it != 0) return false;
    return true;
}

inline void clearArray(ContainerPodValue* begin, const ContainerPodValue* end) {
    for (auto it = begin; it != end; ++it) {
        auto& el = *it;
        clearObject(&el);
    }
}

inline bool isEmptyArray(const ContainerPodValue* begin, const ContainerPodValue* end) {
    for (auto it = begin; it != end; ++it) {
        auto& el = *it;
        if (!isEmptyObject(el)) return false;
    }
    return true;
}


inline void clearObject(ContainerObject* obj) {
    Packet::clearProp(&(obj->charVal));
    Packet::clearProp(&(obj->ucharVal));
    Packet::clearProp(&(obj->intVal));
    Packet::clearProp(&(obj->uintVal));
    Packet::clearProp(&(obj->doubleVal));
    obj->podVector.clear();
    obj->vecVector.clear();
    clearArray(obj->intArray.data(), obj->intArray.data() + obj->intArray.size());
    clearArray(obj->podArray.data(), obj->podArray.data() + obj->podArray.size());
    obj->mapIntInt.clear();
    obj->mapIntStruct.clear();
    obj->mapStringStruct.clear();
    obj->complexVector.clear();
}
inline bool isEmptyObject(const ContainerObject& obj) {
    if (!Packet::isPropEmpty(obj.charVal)) return false;
    if (!Packet::isPropEmpty(obj.ucharVal)) return false;
    if (!Packet::isPropEmpty(obj.intVal)) return false;
    if (!Packet::isPropEmpty(obj.uintVal)) return false;
    if (!Packet::isPropEmpty(obj.doubleVal)) return false;
    if (!obj.podVector.empty()) return false;
    if (!obj.vecVector.empty()) return false;
    if (!isEmptyArray(obj.intArray.data(), obj.intArray.data() + obj.intArray.size())) return false;
    return true;
}

inline void clearObject(ContainerPodValue* obj) {
    Packet::clearProp(&(obj->intVal));
    Packet::clearProp(&(obj->doubleVal));
}
inline bool isEmptyObject(const ContainerPodValue& obj) {
    if (!Packet::isPropEmpty(obj.intVal)) return false;
    if (!Packet::isPropEmpty(obj.doubleVal)) return false;
    return true;
}

inline void clearObject(ContainerVectorValue* obj) {
    obj->podVecVal.clear();
}
inline bool isEmptyObject(const ContainerVectorValue& obj) {
    if (!obj.podVecVal.empty()) return false;
    return true;
}

inline void clearObject(ContainerComplexValue* obj) {
    Packet::clearProp(&(obj->intVal));
    Packet::clearProp(&(obj->doubleVal));
    Packet::clearProp(&(obj->stringVal));
}
inline bool isEmptyObject(const ContainerComplexValue& obj) {
    if (!Packet::isPropEmpty(obj.intVal)) return false;
    if (!Packet::isPropEmpty(obj.doubleVal)) return false;
    if (!Packet::isPropEmpty(obj.stringVal)) return false;
    return true;
}

inline void clearObject(ContainerVectorObjectValue* obj) {
    obj->complexVecVal.clear();
}
inline bool isEmptyObject(const ContainerVectorObjectValue& obj) {
    if (!obj.complexVecVal.empty()) return false;
    return true;
}


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) { return false; }

inline bool isEmptyYaml(std::string_view name) { return name.empty(); }

inline void packObject(const ContainerObject& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ContainerObject* obj);

inline void packObject(const ContainerPodValue& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ContainerPodValue* obj);

inline void packObject(const ContainerVectorValue& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ContainerVectorValue* obj);

inline void packObject(const ContainerComplexValue& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ContainerComplexValue* obj);

inline void packObject(const ContainerVectorObjectValue& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ContainerVectorObjectValue* obj);


inline void packObject(const ContainerObject& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.charVal))
        builder->field("charVal", obj.charVal);
    if (!Packet::isPropEmpty(obj.ucharVal))
        builder->field("ucharVal", obj.ucharVal);
    if (!Packet::isPropEmpty(obj.intVal))
        builder->field("intVal", obj.intVal);
    if (!Packet::isPropEmpty(obj.uintVal))
        builder->field("uintVal", obj.uintVal);
    if (!Packet::isPropEmpty(obj.doubleVal))
        builder->field("doubleVal", obj.doubleVal);
    if (!obj.podVector.empty()) {
        builder->array("podVector");
        for (auto& val: obj.podVector) {
            packObject(val, {}, builder);
        }
        builder->pop();  // podVector

    }

    if (!obj.vecVector.empty()) {
        builder->array("vecVector");
        for (auto& val: obj.vecVector) {
            packObject(val, {}, builder);
        }
        builder->pop();  // vecVector

    }

    if (!isEmptyArray(obj.intArray.data(), obj.intArray.data() + obj.intArray.size())) {
        builder->flowSeqVec("intArray", obj.intArray);
    }

    if (!isEmptyArray(obj.podArray.data(), obj.podArray.data() + obj.podArray.size())) {
        builder->array("podArray");
        for (auto& val: obj.podArray) {
            packObject(val, {}, builder);
        }
        builder->pop();  // podArray

    }

    if (!obj.mapIntInt.empty()) {
        builder->array("mapIntInt");
        for (auto& [key, val]: obj.mapIntInt) {
            builder->map();
            builder->field("key", key);
            builder->field("val", val);
            builder->pop();
        }
        builder->pop();  // mapIntInt
    }

    if (!obj.mapIntStruct.empty()) {
        builder->array("mapIntStruct");
        for (auto& [key, val]: obj.mapIntStruct) {
            builder->map();
            builder->field("key", key);
            packObject(val, "val", builder);
            builder->pop();
        }
        builder->pop();  // mapIntStruct
    }

    if (!obj.mapStringStruct.empty()) {
        builder->array("mapStringStruct");
        for (auto& [key, val]: obj.mapStringStruct) {
            builder->map();
            builder->field("key", key);
            packObject(val, "val", builder);
            builder->pop();
        }
        builder->pop();  // mapStringStruct
    }

    if (!obj.complexVector.empty()) {
        builder->array("complexVector");
        for (auto& val: obj.complexVector) {
            packObject(val, {}, builder);
        }
        builder->pop();  // complexVector

    }

    builder->pop();  // ContainerObject

}

inline void unpackObject(const apl::YamlNode& node, ContainerObject* obj) {

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

        if ("doubleVal" == objNode.key) {
            objNode.get(&obj->doubleVal);
            continue;
        }

        if ("podVector" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->podVector.resize(size);
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->podVector[i]);
            }
            continue;
        }

        if ("vecVector" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->vecVector.resize(size);
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->vecVector[i]);
            }
            continue;
        }

        if ("intArray" == objNode.key) {
            auto size = ssize(objNode.nodes);
            auto arraySize = ssize(obj->intArray);
            if (arraySize < size) size = arraySize;
            for (Index i = 0; i < size; ++i) {
                objNode.nodes[i].get(&obj->intArray[i]);
            }
            continue;
        }

        if ("podArray" == objNode.key) {
            auto size = ssize(objNode.nodes);
            auto arraySize = ssize(obj->podArray);
            if (arraySize < size) size = arraySize;
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->podArray[i]);
            }
            continue;
        }

        if ("mapIntInt" == objNode.key) {
            for(auto& mapNode: objNode) {
                decltype(obj->mapIntInt)::key_type key{};
                decltype(obj->mapIntInt)::value_type::second_type val{};

                for(auto& valNode: mapNode) {
                    if ("key" == valNode.key) valNode.get(&key);
                    if ("val" == valNode.key) {
                        valNode.get(&val);
                    }
                }
                obj->mapIntInt.emplace(key, std::move(val));
            }  // for nodes
            continue;
        }

        if ("mapIntStruct" == objNode.key) {
            for(auto& mapNode: objNode) {
                decltype(obj->mapIntStruct)::key_type key{};
                decltype(obj->mapIntStruct)::value_type::second_type val{};

                for(auto& valNode: mapNode) {
                    if ("key" == valNode.key) valNode.get(&key);
                    if ("val" == valNode.key) {
                        unpackObject(valNode, &val);
                    }
                }
                obj->mapIntStruct.emplace(key, std::move(val));
            }  // for nodes
            continue;
        }

        if ("mapStringStruct" == objNode.key) {
            for(auto& mapNode: objNode) {
                decltype(obj->mapStringStruct)::key_type key{};
                decltype(obj->mapStringStruct)::value_type::second_type val{};

                for(auto& valNode: mapNode) {
                    if ("key" == valNode.key) valNode.get(&key);
                    if ("val" == valNode.key) {
                        unpackObject(valNode, &val);
                    }
                }
                obj->mapStringStruct.emplace(key, std::move(val));
            }  // for nodes
            continue;
        }

        if ("complexVector" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->complexVector.resize(size);
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->complexVector[i]);
            }
            continue;
        }

    }  // for nodes

}

inline void packObject(const ContainerPodValue& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.intVal))
        builder->field("intVal", obj.intVal);
    if (!Packet::isPropEmpty(obj.doubleVal))
        builder->field("doubleVal", obj.doubleVal);
    builder->pop();  // ContainerPodValue

}

inline void unpackObject(const apl::YamlNode& node, ContainerPodValue* obj) {

    for (auto& objNode: node) {
        if ("intVal" == objNode.key) {
            objNode.get(&obj->intVal);
            continue;
        }

        if ("doubleVal" == objNode.key) {
            objNode.get(&obj->doubleVal);
            continue;
        }

    }  // for nodes

}

inline void packObject(const ContainerVectorValue& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!obj.podVecVal.empty()) {
        builder->flowSeqVec("podVecVal", obj.podVecVal);
    }

    builder->pop();  // ContainerVectorValue

}

inline void unpackObject(const apl::YamlNode& node, ContainerVectorValue* obj) {

    for (auto& objNode: node) {
        if ("podVecVal" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->podVecVal.resize(size);
            for (Index i = 0; i < size; ++i) {
                objNode.nodes[i].get(&obj->podVecVal[i]);
            }
            continue;
        }

    }  // for nodes

}

inline void packObject(const ContainerComplexValue& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.intVal))
        builder->field("intVal", obj.intVal);
    if (!Packet::isPropEmpty(obj.doubleVal))
        builder->field("doubleVal", obj.doubleVal);
    if (!Packet::isPropEmpty(obj.stringVal))
        builder->field("stringVal", obj.stringVal);
    builder->pop();  // ContainerComplexValue

}

inline void unpackObject(const apl::YamlNode& node, ContainerComplexValue* obj) {

    for (auto& objNode: node) {
        if ("intVal" == objNode.key) {
            objNode.get(&obj->intVal);
            continue;
        }

        if ("doubleVal" == objNode.key) {
            objNode.get(&obj->doubleVal);
            continue;
        }

        if ("stringVal" == objNode.key) {
            objNode.get(&obj->stringVal);
            continue;
        }

    }  // for nodes

}

inline void packObject(const ContainerVectorObjectValue& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!obj.complexVecVal.empty()) {
        builder->array("complexVecVal");
        for (auto& val: obj.complexVecVal) {
            packObject(val, {}, builder);
        }
        builder->pop();  // complexVecVal

    }

    builder->pop();  // ContainerVectorObjectValue

}

inline void unpackObject(const apl::YamlNode& node, ContainerVectorObjectValue* obj) {

    for (auto& objNode: node) {
        if ("complexVecVal" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->complexVecVal.resize(size);
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->complexVecVal[i]);
            }
            continue;
        }

    }  // for nodes

}

}  // namespace impl_ContainerObject_yaml


inline void pack(const ContainerObject& obj, apl::YamlBuilder* builder) {
    builder->begin();
    impl_ContainerObject_yaml::packObject(obj, "ContainerObject", builder);
    builder->end();
}

inline void unpack(const apl::Yaml& root, ContainerObject* obj) {
    impl_ContainerObject_yaml::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_ContainerObject_yaml::unpackObject(docNode, obj);
    }
}

}  // namespace container_test

