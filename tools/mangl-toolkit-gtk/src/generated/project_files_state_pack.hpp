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

inline void pack(const ProjectFilesState& obj, apl::YamlBuilder* builder);
inline void unpack(const apl::Yaml& packet, ProjectFilesState* obj);

inline auto packStringYaml(const ProjectFilesState& obj) {
    apl::YamlBuilder builder;
    pack(obj, &builder);
    return builder.str();
}

inline auto unpackStringYaml(std::string_view str, ProjectFilesState* obj) {
    apl::Yaml packet;
    apl::YamlParser parser;
    parser.parse(str, &packet);
    unpack(packet, obj);
}

inline auto unpackDataYaml(typename apl::DataParam<>::Arg data, ProjectFilesState* obj) {
    unpackStringYaml({(const char *)data.data(), data.size()}, obj);
}

inline auto packFileYaml(std::string_view filename, const ProjectFilesState& obj) {
    auto str = packStringYaml(obj);
    apl::saveFile(filename, str);
}

inline auto unpackFileYaml(std::string_view filename, ProjectFilesState* obj) {
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}

inline auto unpackFileIfExistsYaml(std::string_view filename, ProjectFilesState* obj) {
        if (!apl::fileExists(filename)) return;
        auto str = apl::loadFile(filename);
        unpackStringYaml(str, obj);
}



namespace impl_ProjectFilesState_yaml {

using namespace mangl;

using ::apl::Index;
using std::ssize;

inline void clearObject(ProjectFilesState* obj);
inline bool isEmptyObject(const ProjectFilesState& obj);
inline void clearObject(ProjectCategoryState* obj);
inline bool isEmptyObject(const ProjectCategoryState& obj);
inline void clearObject(ProjectFileState* obj);
inline bool isEmptyObject(const ProjectFileState& obj);

inline void clearArray(ProjectCategoryState* begin, const ProjectCategoryState* end);
inline bool isEmptyArray(const ProjectCategoryState* begin, const ProjectCategoryState* end);

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

inline void clearArray(ProjectCategoryState* begin, const ProjectCategoryState* end) {
    for (auto it = begin; it != end; ++it) {
        auto& el = *it;
        clearObject(&el);
    }
}

inline bool isEmptyArray(const ProjectCategoryState* begin, const ProjectCategoryState* end) {
    for (auto it = begin; it != end; ++it) {
        auto& el = *it;
        if (!isEmptyObject(el)) return false;
    }
    return true;
}


inline void clearObject(ProjectFilesState* obj) {
    obj->selectedCat = {};
    clearArray(obj->fileContainers.data(), obj->fileContainers.data() + obj->fileContainers.size());
}
inline bool isEmptyObject(const ProjectFilesState& obj) {
    if (int(obj.selectedCat) != int(0)) return false;
    if (!isEmptyArray(obj.fileContainers.data(), obj.fileContainers.data() + obj.fileContainers.size())) return false;
    return true;
}

inline void clearObject(ProjectCategoryState* obj) {
    Packet::clearProp(&(obj->selectedIndex), -1);
    Packet::clearProp(&(obj->currentDirectory));
    obj->files.clear();
}
inline bool isEmptyObject(const ProjectCategoryState& obj) {
    if (!Packet::isPropEmpty(obj.selectedIndex)) return false;
    if (!Packet::isPropEmpty(obj.currentDirectory)) return false;
    if (!obj.files.empty()) return false;
    return true;
}

inline void clearObject(ProjectFileState* obj) {
    Packet::clearProp(&(obj->title));
    Packet::clearProp(&(obj->name));
    Packet::clearProp(&(obj->path));
}
inline bool isEmptyObject(const ProjectFileState& obj) {
    if (!Packet::isPropEmpty(obj.title)) return false;
    if (!Packet::isPropEmpty(obj.name)) return false;
    if (!Packet::isPropEmpty(obj.path)) return false;
    return true;
}


template <typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
bool isEmptyYaml(T name) { return false; }

inline bool isEmptyYaml(std::string_view name) { return name.empty(); }

inline void packObject(const ProjectFilesState& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ProjectFilesState* obj);

inline void packObject(const ProjectCategoryState& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ProjectCategoryState* obj);

inline void packObject(const ProjectFileState& obj, std::string_view name, apl::YamlBuilder* builder);
inline void unpackObject(const apl::YamlNode& node, ProjectFileState* obj);


inline void packObject(const ProjectFilesState& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (static_cast<int>(obj.selectedCat) != 0)
        builder->field("selectedCat", static_cast<int>(obj.selectedCat));
    if (!isEmptyArray(obj.fileContainers.data(), obj.fileContainers.data() + obj.fileContainers.size())) {
        builder->array("fileContainers");
        for (auto& val: obj.fileContainers) {
            packObject(val, {}, builder);
        }
        builder->pop();  // fileContainers

    }

    builder->pop();  // ProjectFilesState

}

inline void unpackObject(const apl::YamlNode& node, ProjectFilesState* obj) {

    for (auto& objNode: node) {
        if ("selectedCat" == objNode.key) {
            int val;
            objNode.get(&val);
            obj->selectedCat = static_cast<ProjectCategory>(val);
            continue;
        }

        if ("fileContainers" == objNode.key) {
            auto size = ssize(objNode.nodes);
            auto arraySize = ssize(obj->fileContainers);
            if (arraySize < size) size = arraySize;
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->fileContainers[i]);
            }
            continue;
        }

    }  // for nodes

}

inline void packObject(const ProjectCategoryState& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropDefault(obj.selectedIndex, -1))
        builder->field("selectedIndex", obj.selectedIndex);
    if (!Packet::isPropEmpty(obj.currentDirectory))
        builder->field("currentDirectory", obj.currentDirectory);
    if (!obj.files.empty()) {
        builder->array("files");
        for (auto& val: obj.files) {
            packObject(val, {}, builder);
        }
        builder->pop();  // files

    }

    builder->pop();  // ProjectCategoryState

}

inline void unpackObject(const apl::YamlNode& node, ProjectCategoryState* obj) {

    for (auto& objNode: node) {
        if ("selectedIndex" == objNode.key) {
            objNode.get(&obj->selectedIndex);
            continue;
        }

        if ("currentDirectory" == objNode.key) {
            objNode.get(&obj->currentDirectory);
            continue;
        }

        if ("files" == objNode.key) {
            auto size = ssize(objNode.nodes);
            obj->files.resize(size);
            for (Index i = 0; i < size; ++i) {
                unpackObject(objNode.nodes[i], &obj->files[i]);
            }
            continue;
        }

    }  // for nodes

}

inline void packObject(const ProjectFileState& obj, std::string_view name, apl::YamlBuilder* builder) {

    if (isEmptyYaml(name)) {
        builder->map();
    } else {
        builder->obj(name);
    }
    if (!Packet::isPropEmpty(obj.title))
        builder->field("title", obj.title);
    if (!Packet::isPropEmpty(obj.name))
        builder->field("name", obj.name);
    if (!Packet::isPropEmpty(obj.path))
        builder->field("path", obj.path);
    builder->pop();  // ProjectFileState

}

inline void unpackObject(const apl::YamlNode& node, ProjectFileState* obj) {

    for (auto& objNode: node) {
        if ("title" == objNode.key) {
            objNode.get(&obj->title);
            continue;
        }

        if ("name" == objNode.key) {
            objNode.get(&obj->name);
            continue;
        }

        if ("path" == objNode.key) {
            objNode.get(&obj->path);
            continue;
        }

    }  // for nodes

}

}  // namespace impl_ProjectFilesState_yaml


inline void pack(const ProjectFilesState& obj, apl::YamlBuilder* builder) {
    builder->begin();
    impl_ProjectFilesState_yaml::packObject(obj, "ProjectFilesState", builder);
    builder->end();
}

inline void unpack(const apl::Yaml& root, ProjectFilesState* obj) {
    impl_ProjectFilesState_yaml::clearObject(obj);
    for (auto& docNode: root.front()) {
        impl_ProjectFilesState_yaml::unpackObject(docNode, obj);
    }
}

}  // namespace toolkit

