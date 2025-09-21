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

inline void pack(const ScreenLayoutState& obj, apl::YamlBuilder* yaml);
inline void unpack(const apl::Yaml& yaml, ScreenLayoutState* obj);


namespace impl_screenLayoutState_yaml {


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
    if(obj.orientation != mangl::M::Orientation0){
        yaml->field("orientation",(int)obj.orientation);
    }
    if (obj.zoom != 0)
        yaml->field("zoom", obj.zoom);
    yaml->pop();

}
inline void packObject(const undefined& obj, std::integral auto name, apl::YamlBuilder* yaml) {

    if (isEmptyYaml(name)) {
        yaml->map();
    } else {
        yaml->obj(name);
    }
    if(obj.orientation != mangl::M::Orientation0){
        yaml->field("orientation",(int)obj.orientation);
    }
    if (obj.zoom != 0)
        yaml->field("zoom", obj.zoom);
    yaml->pop();

}
inline void unpackObject(const apl::YamlNode& node, undefined* obj){

    for (auto& objNode: node){
        if ("orientation" == objNode.key) {
            objNode.get(&(obj->orientation));
            continue;
        }
        if ("zoom" == objNode.key) {
            objNode.get(&(obj->zoom));
            continue;
        }
    }

}

}  // namespace impl_screenLayoutState_yaml


inline std::string packString(const ScreenLayoutState& obj){
    apl::YamlBuilder yaml;
    pack(obj, &yaml);
    return yaml.str();
}

inline void packFile(const ScreenLayoutState& obj, std::string_view fileName){
    auto strYaml = packString(obj);
    apl::saveFile(fileName,strYaml);
}

inline void unpack(const apl::Yaml& yaml, ScreenLayoutState* obj){
    for (auto& doc : yaml.documents) {
         for (auto& docNode: doc) {
             impl_screenLayoutState_yaml::unpackObject(docNode, obj );
         }
   }
}

inline void unpackYaml(std::string_view text, ScreenLayoutState* obj){ 
    auto yaml = apl::parseYaml(text);
    unpack(yaml, obj);
}



inline void pack(const ScreenLayoutState& obj, apl::YamlBuilder* yaml){
    yaml->begin();
    impl_screenLayoutState_yaml::packObject(obj, "screenLayoutState", yaml);
    yaml->end();
}

