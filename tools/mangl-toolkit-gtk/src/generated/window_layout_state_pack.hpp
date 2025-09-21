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

#include <apeal/data_param.hpp>
#include <apeal/index.hpp>
#include <apeal/parse/yaml.hpp>
#include <mangl/packet.h>
#include <string>
#include <string_view>
#include <apeal/file.hpp>

namespace toolkit {

inline void pack(const WindowLayoutState& obj, apl::YamlBuilder* builder);
inline void unpack(const apl::Yaml& packet, WindowLayoutState* obj);

inline auto packStringYaml(const WindowLayoutState& obj) {
    apl::YamlBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringYaml(std::string_view str, WindowLayoutState* obj) {
    apl::Yaml packet;
    apl::YamlParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto unpackDataYaml(typename apl::DataParam<>::Arg data, WindowLayoutState* obj) {
    unpackStringYaml({(const char *)data.data(), data.size()}, obj);
}

inline auto packFileYaml(std::string_view filename, const WindowLayoutState& obj) {
    auto str = packStringYaml(obj);
    apl::saveFile(filename, str);
}

inline auto unpackFileYaml(std::string_view filename, WindowLayoutState* obj) {
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}

inline auto unpackFileIfExistsYaml(std::string_view filename, WindowLayoutState* obj) {
        if (!apl::fileExists(filename)) return;
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}



namespace impl_WindowLayoutState_yaml {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(WindowLayoutState* obj);
inline bool isEmptyObject(const WindowLayoutState& obj);
inline void clearObject(SizeI* obj);
inline bool isEmptyObject(const SizeI& obj);


inline void clearObject(WindowLayoutState* obj) {
    clearObject(&(obj->size));
}
inline bool isEmptyObject(const WindowLayoutState& obj) {
    isEmptyObject(obj.size);
    return true;
}

inline void clearObject(SizeI* obj) {
    Packet::clearProp(&(obj->w));
    Packet::clearProp(&(obj->h));
}
inline bool isEmptyObject(const SizeI& obj) {
    if (!Packet::isPropEmpty(obj.w)) return false;
    if (!Packet::isPropEmpty(obj.h)) return false;
    return true;
}


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) { return false; }

inline bool isEmptyYaml(std::string_view name) { return name.empty(); }

inline void packObject(const WindowLayoutState& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, WindowLayoutState* obj);

inline void packObject(const SizeI& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, SizeI* obj);


inline void packObject(const WindowLayoutState& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!isEmptyObject(obj.size))
        packObject(obj.size, "size", builder);
    builder->pop();  // WindowLayoutState

}

inline void unpackObject(const apl::YamlNode& node, WindowLayoutState* obj) {

    for (auto& objNode: node) {
        if ("size" == objNode.key) {
            unpackObject(objNode, &obj->size);
            continue;
        }

    }  // for nodes

}

inline void packObject(const SizeI& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.w))
        builder->field("w", obj.w);
    if (!Packet::isPropEmpty(obj.h))
        builder->field("h", obj.h);
    builder->pop();  // SizeI

}

inline void unpackObject(const apl::YamlNode& node, SizeI* obj) {

    for (auto& objNode: node) {
        if ("w" == objNode.key) {
            objNode.get(&obj->w);
            continue;
        }

        if ("h" == objNode.key) {
            objNode.get(&obj->h);
            continue;
        }

    }  // for nodes

}

}  // namespace impl_WindowLayoutState_yaml


inline void pack(const WindowLayoutState& obj, apl::YamlBuilder* builder) {
    builder->begin();
    impl_WindowLayoutState_yaml::packObject(obj, "WindowLayoutState", builder);
    builder->end();
}

inline void unpack(const apl::Yaml& root, WindowLayoutState* obj) {
    impl_WindowLayoutState_yaml::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_WindowLayoutState_yaml::unpackObject(docNode, obj);
    }
}

}  // namespace toolkit

