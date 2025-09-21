/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_blueprint_structure.h"

#include <apeal/parse/yaml.hpp>
#include "objects_parse.h"


#include <set>

namespace mangl::blueprint {

class Loader {
private:
    using YamlDoc = apl::Yaml::Document;
    using YamlDocA = const YamlDoc&;

    using YamlNode = apl::Yaml::Node;
    using YamlNodeA = const YamlNode&;
public:
    using ImportDirs = std::vector<String>;

public:
    // Loads dependencies
    const DepsContainer& loadDeps(StrView filename, BlueprintP blueprint);
    const DepsContainer& getDeps() const { return deps_; }

    void load(StrView filename, BlueprintP blueprint);
    void loadScreen(YamlNodeA node, BlueprintP blueprint);
//    void loadWorld(YamlNodeA node,Screen& screen, BlueprintP blueprint);
//    void loadComponent(YamlNodeA node,World& world, BlueprintP blueprint);
//    void loadExpression(YamlNodeA& node, Expression& exp);
    //RealGL evaluateExpression(Expression& exp);
    void loadOp(YamlNodeA& node, Op& op);
    void loadFunction(StrView filename);

   /* Rect calcCoords(const Component& elem, const Screen& screen);
    Point calcPointCoords(const Component& elem, const Screen& screen);*/
    //void createScreenComponents(Index screenIndex, const TextureAtlas& gameTex);

    CoordMajor parseCoords(String str, RealGL max);
    //Color parseColor(StrView color);
    //std::set<String> loadFontNames(const Project& project);

    void parseHorzAlign(YamlNodeA node,StrView value, int& alignHorz);
    void parseVertAlign(YamlNodeA node,StrView value, int& alignVert);

    void clear() noexcept {
        //*this = {}; 
        mainModule_.clear();
        deps_.clear();
        importDirs_.clear();
        blueprint_ = nullptr;
        while (!parseFileStack_.empty())
            parseFileStack_.pop();
    }

private:
   /* using YamlDoc = apl::Yaml::Document;
    using YamlDocA = const YamlDoc&;

    using YamlNode = apl::Yaml::Node;
    using YamlNodeA = const YamlNode&;*/

    struct Module {
        String filename;
        apl::Yaml parsedData;

        std::vector<Module> imports;

        void clear() noexcept {
            filename.clear();
            parsedData.clear();
            imports.clear();
        }
    };

    using ModuleP = Module*;

    Module mainModule_;
    DepsContainer deps_;
    ImportDirs importDirs_;
    Blueprint* blueprint_{};
    std::stack<String> parseFileStack_;

    Module parseModule(StrView filename, DepP parent);
    void parseModuleTags(StrView filename, YamlNodeA yaml, ModuleP mod, DepP parent);

    String locateModule(YamlNodeA yaml, StrView filename, DepP parent);
    String locateModule(StrView filename, DepP parent) {
        YamlNode dummy;
        dummy.line = -1;
        return locateModule(dummy, filename, parent);
    }

    // Header parsing
    void parseDocumentHeader(StrView filename, YamlDocA doc, ModuleP mod, DepP parent);
    void parseHeaderImports(StrView filename, YamlNodeA yaml, ModuleP mod, DepP parent);
    void parseHeaderVars(YamlNodeA yaml);


    // Texture parsing
    void parseDocumentTexture(StrView filename, YamlDocA doc);

    // Node parsing
    void parseDocumentNode(StrView filename, YamlDocA doc);
    
    void processNodeTypeBox(YamlNodeA yaml, NodeP node);
    void processNodeTypeImage(YamlNodeA yaml, NodeP node);
    //void processNodePosition(YamlNodeA& yaml, NodeP node);

    LayoutPoint parseNodePosition(YamlNodeA yaml, NodeP node);
    SourceSize parseNodeSize(YamlNodeA yaml, NodeP node);
    LayoutOrigin parseOrigin(YamlNodeA yaml, NodeP node);
    LayoutAlignment parseAlign(YamlNodeA yaml, NodeP node);

    void parseSpacialValue(YamlNodeA yaml, StrView text, NodeP node, SpacialValueP value);
    void parseSpacialValueText(YamlNodeA yaml, StrView text, NodeP node, SpacialValueP value);
    //void parseSpacialValue(YamlNodeA yaml, NodeP node, SpacialValueP value) {
    //    parseSpacialValue(yaml, yaml.value, node, value);
    //}

    auto parseLayoutMeasureHorz(YamlNodeA yaml, StrView text, NodeP node) {
        LayoutMeasureX m;
        parseSpacialValue(yaml, text, node, &m);
        return m;
    }

    auto parseLayoutMeasureVert(YamlNodeA yaml, StrView text, NodeP node) {
        LayoutMeasureY m;
        parseSpacialValue(yaml, text, node, &m);
        return m;
    }

    auto parseLayoutMeasureHorz(YamlNodeA yaml, NodeP node) {
        return parseLayoutMeasureHorz(yaml, yaml.getValue(), node);
    }

    auto parseLayoutMeasureVert(YamlNodeA yaml, NodeP node) {
        return parseLayoutMeasureVert(yaml, yaml.getValue(), node);
    }


    BlueprintColor parseLayoutColor(YamlNodeA yaml, NodeP node);
    int parseLayoutColors(YamlNodeA yaml, NodeP node, ColorsSet& colors);


    //
    // Error handling
    //
    auto makeErrorPrefix(YamlNodeA yaml) noexcept {
        using namespace apl;
        auto line = yaml.line + 1;
        auto filename = extractFileName(parseFileStack_.top());
        return fmt("%d:%s", line, filename);
    }

    [[noreturn]]
    void raiseParseError(YamlNodeA yaml, StrView text) {
        auto&& prefix = makeErrorPrefix(yaml);
        apl::throwError("%s:%s", prefix, text);
    }

    template <class ...Args>
    [[noreturn]]
    void raiseParseError(YamlNodeA yaml, StrView text, Args&& ...args) {
        raiseParseError(yaml, apl::fmt(text, std::forward<Args>(args)...));
    }

    [[noreturn]]
    void raiseErrorInvalidKey(YamlNodeA yaml) {
        apl::throwError("%s:Invalid key: %s", makeErrorPrefix(yaml), yaml.key);
    }

    template <class Condition, class ...Args>
    void verifySource(Condition cond, YamlNodeA yaml, Args&& ...args) {
        using namespace apl;
        if (isFalse(cond))
            raiseParseError(yaml, std::forward<Args>(args)...);
    }
};

} // namespace mangl::blueprint
