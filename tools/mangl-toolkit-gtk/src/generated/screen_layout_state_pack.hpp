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

inline void pack(const ScreenLayoutState& obj, apl::YamlBuilder* builder);
inline void unpack(const apl::Yaml& packet, ScreenLayoutState* obj);

inline auto packStringYaml(const ScreenLayoutState& obj) {
    apl::YamlBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringYaml(std::string_view str, ScreenLayoutState* obj) {
    apl::Yaml packet;
    apl::YamlParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto unpackDataYaml(typename apl::DataParam<>::Arg data, ScreenLayoutState* obj) {
    unpackStringYaml({(const char *)data.data(), data.size()}, obj);
}

inline auto packFileYaml(std::string_view filename, const ScreenLayoutState& obj) {
    auto str = packStringYaml(obj);
    apl::saveFile(filename, str);
}

inline auto unpackFileYaml(std::string_view filename, ScreenLayoutState* obj) {
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}

inline auto unpackFileIfExistsYaml(std::string_view filename, ScreenLayoutState* obj) {
        if (!apl::fileExists(filename)) return;
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}



namespace impl_ScreenLayoutState_yaml {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(ScreenLayoutState* obj);
inline bool isEmptyObject(const ScreenLayoutState& obj);


inline void clearObject(ScreenLayoutState* obj) {
    obj->orientation = mangl::M::Orientation0;
    Packet::clearProp(&(obj->zoom));
}
inline bool isEmptyObject(const ScreenLayoutState& obj) {
    if (int(obj.orientation) != int(mangl::M::Orientation0)) return false;
    if (!Packet::isPropEmpty(obj.zoom)) return false;
    return true;
}


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) { return false; }

inline bool isEmptyYaml(std::string_view name) { return name.empty(); }

inline void packObject(const ScreenLayoutState& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ScreenLayoutState* obj);


inline void packObject(const ScreenLayoutState& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (static_cast<int>(obj.orientation) != mangl::M::Orientation0)
        builder->field("orientation", static_cast<int>(obj.orientation));
    if (!Packet::isPropEmpty(obj.zoom))
        builder->field("zoom", obj.zoom);
    builder->pop();  // ScreenLayoutState

}

inline void unpackObject(const apl::YamlNode& node, ScreenLayoutState* obj) {

    for (auto& objNode: node) {
        if ("orientation" == objNode.key) {
            int val;
            objNode.get(&val);
            obj->orientation = static_cast<mangl::M::Orientation>(val);
            continue;
        }

        if ("zoom" == objNode.key) {
            objNode.get(&obj->zoom);
            continue;
        }

    }  // for nodes

}

}  // namespace impl_ScreenLayoutState_yaml


inline void pack(const ScreenLayoutState& obj, apl::YamlBuilder* builder) {
    builder->begin();
    impl_ScreenLayoutState_yaml::packObject(obj, "ScreenLayoutState", builder);
    builder->end();
}

inline void unpack(const apl::Yaml& root, ScreenLayoutState* obj) {
    impl_ScreenLayoutState_yaml::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_ScreenLayoutState_yaml::unpackObject(docNode, obj);
    }
}

}  // namespace toolkit

