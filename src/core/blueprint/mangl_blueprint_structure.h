/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_types.h"
#include "../mangl_scene.h"
#include "../mangl_widgets.h"
#include "../mangl_texture_atlas.h"

#include "screen_parse.h"
#include "objects_parse.h"


#include "../effects/mangl_blink_effect.h"

#include <variant>

namespace mangl::blueprint {

using BlueprintHash = StringPointerContainer<void>::Hash;
struct Variable;
using VariableP = Variable*;

enum class Axis {
    X, Y, Z
};

enum class ValueType {
    None, Bool, Int, Real, String, Variable
};

enum class MeasureUnit {
    Immediate, Point, Percent
};

struct Value {
    using Type = ValueType;
    Type type{Type::None};

    using Units = MeasureUnit;
    Units units{Units::Immediate};

    std::variant<bool, int, Real, String, VariableP> value;
    String source;
    bool valid{}, evaluated{};


    void setSource(StrView text) noexcept {
        source.assign(text.data(), text.size());
        evaluated = false;
        valid = true;
    }

    void invalidate() noexcept { 
        valid = evaluated = false;
    }

    void setPending() noexcept {
        evaluated = false;
    }


    void setReal(Real v) noexcept {
        type = Type::Real;
        assignValue(v);
    }

    void setString(StrView text) noexcept {
        type = Type::String;
        assignValue(String{text});
    }

    void setVariable(VariableP var) noexcept;


    Real getReal() const noexcept {
        ADebugTrapAssert(valid && evaluated);
        if (Type::Real == type)
            return std::get<Real>(value);
        if (Type::Variable == type)
            return getVariableReal();
        ADebugTrap();
        return {};
    }

    String getString() const noexcept {
        if (Type::String == type)
            return std::get<String>(value);
        if (Type::Variable == type)
            return getVariableString();
        ADebugTrap();
        return {};
    }


    Real getVariableReal() const noexcept;
    String getVariableString() const noexcept;

private:
    template <class V>
    void assignValue(V&& val) noexcept {
        value = val;
        valid = evaluated = true;
    }
};

//enum LayoutAlignment {
//    Align
//};

using LayoutAlignment = uint64_t;

struct LayoutOriginAxis {
    //int align;
    Value value;
};

struct LayoutOrigin {
    LayoutAlignment align{};
    LayoutOriginAxis x, y; //{.align = M::AlignLeft}, vert{.align = M::AlignBottom};
};

//struct LayoutAlignment {
//    int horz{M::AlignLeft}, vert{M::AlignBottom};
//};


struct Node;
using NodeP = Node*;

struct SpacialValue: Value {
    //using Value::Type;

    //Real realValue{};
    //Value value;
    //MeasureUnit units{MeasureUnit::Point};
    Axis axis{};
    //bool valid{};

    //bool haveEvalReal{};
    //Real evalReal;

    //void setEvalReal(Real v) noexcept {
    //    haveEvalReal = true;
    //    evalReal = v;
    //}
};

using SpacialValueP = SpacialValue*;


struct TimeValue {
};

//struct LayoutCoordinate: SpacialValue {
//};


struct LayoutMeasureX: SpacialValue {
    LayoutMeasureX() noexcept {
        axis = Axis::X;
    }
};

using LayoutMeasureXA = const LayoutMeasureX&;
using LayoutMeasureXP = LayoutMeasureX*;

struct LayoutMeasureY: SpacialValue {
    LayoutMeasureY() noexcept {
        axis = Axis::Y;
    }
};

using LayoutMeasureYA = const LayoutMeasureY&;
using LayoutMeasureYP = LayoutMeasureY*;


struct LayoutPoint {
    bool valid{};
    LayoutMeasureX x;
    LayoutMeasureY y;
};
using LayoutPointA = const LayoutPoint&;
using LayoutPointP = LayoutPoint*;

struct SourceSize {
    bool valid{};
    LayoutMeasureX w;
    LayoutMeasureY h;
};
using LayoutSizeA = const SourceSize&;
using LayoutSizeP = SourceSize*;


struct BlueprintColor {
    NodeP ref{};
    bool valid{};
    Color value{};
};
using BlueprintColorA = const BlueprintColor&;


struct BlueprintTexture {
    String name, file;
    BlueprintHash nameHash{};
    const TextureAtlas* atlas{};
};

using BlueprintTextureA = const BlueprintTexture&;
using BlueprintTextureCP = const BlueprintTexture*;
using TexturesContainer = StringPointerContainer<BlueprintTexture>;



enum class NodeType {
    Unknown = 0,
    Box, Rectangle, Image, Texture = Image,
};


struct EvalRect {
    bool evaluated{};
    Rect value{};

    auto& operator=(RectA r) noexcept {
        evaluated = true;
        value = r;
        return *this;
    }

    auto& operator()() const noexcept { return value; }
};

using ColorsSet = std::array<BlueprintColor, 4>;

struct Node;
using NodeP = Node*;


struct Node {
    //
    // Node setup
    //

    // Identification
    String id;
    Hash idHash{};

    // Hierarchy
    NodeP parent{};
    NodeP spacialParent{};
    NodeP timeParent{};

    // Node classification
    NodeType type{};

    // Setup
    LayoutOrigin origin;
    LayoutAlignment align{};
    LayoutPoint position;
    SourceSize size;
    ColorsSet colors;
    int numColors{};
    BlueprintTextureCP atlasRef;
    String atlasName, textureName;
    Texture texture;
    SourceSize elementSize;


    //
    // Runtime
    //
    EvalRect evalRect;  // computed rect
    PrimP prim{};
    LabelP label{};
    BlinkEffectP blinkEffect{};
    Index index{};  // Node general index

    // Debugging
    PrimP debugBoundsPrim{};
    LabelP debugIdLabel{};
};


using NodesContainer = StringPointerContainer<Node>;


struct Uniform {
    String value;
};

using UniformsContainer = StringPointerContainer<Uniform>;


struct Variable {
    Value value;
    String name;
    String notes;
};
using VariablesContainer = StringPointerContainer<Variable>;



struct Dep;
using DepP = Dep*;

struct Dep {
    String path;
    Index line{};
    DepP from{};  // included from
};



using DepsContainer = StringPointerContainer<Dep>;

struct Blueprint {
    UniformsContainer uniforms;
    NodesContainer nodes;
    std::vector<NodeP> orderedNodes;
    TexturesContainer textures;
    VariablesContainer variables;


    void clear() noexcept { 
        variables.clear();
        uniforms.clear();
        textures.clear();

        orderedNodes.clear();
        nodes.clear();

    }

    String id;
    String title;
    // ScreenLayout layout;
    // std::vector<Screen> screens;
    std::vector<Function> Functions;
};

using BlueprintP = Blueprint*;

}  // namespace mangl::blueprint
