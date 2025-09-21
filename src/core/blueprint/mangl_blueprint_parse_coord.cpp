/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#include "mangl_blueprint_loader.h"

#include "../mangl_parse.h"

#include <apeal/parse/vector.hpp>
#include <apeal/str/case.hpp>

namespace mangl::blueprint {

using namespace apl;
using namespace apl::literals;


LayoutOrigin Loader::parseOrigin(YamlNodeA yaml, NodeP node)
{
    LayoutOrigin origin{};

    origin.align = parseAlign(yaml, node);
    return origin;
}

LayoutAlignment Loader::parseAlign(YamlNodeA yaml, NodeP node)
{
    LayoutAlignment align{};

    if (!yaml.value.empty()) {
        auto [horz, vert] = str_split_trim(yaml.value, ',');

        if (!horz.empty()) {
            switch (pshHash(str_lower(horz))) {
            case "left"_psh: align |= M::AlignLeft; break;
            case "center"_psh: align |= M::AlignCenter; break;
            case "right"_psh: align |= M::AlignRight; break;
            default: raiseParseError(yaml, "Invalid align: %s", horz);
            }
        }

        if (!vert.empty()) {
            switch (pshHash(str_lower(vert))) {
            case "bottom"_psh: align |= M::AlignBottom; break;
            case "middle"_psh:
            case "center"_psh: align |= M::AlignVCenter; break;
            case "top"_psh: align |= M::AlignTop; break;
            default: raiseParseError(yaml, "Invalid origin: %s", vert);
            }
        }
    }

    //for (auto& el: yaml) {
    //    switch (pshHash(el.key)) {
    //    }
    //}

    return align;
}


LayoutPoint Loader::parseNodePosition(YamlNodeA yaml, NodeP node)
{
    LayoutPoint pt{};

    if (!yaml.isNull()) {
        auto [xs, ys] = str_split_trim(yaml.getValue(), ',');
        pt.x = parseLayoutMeasureHorz(yaml, xs, node);
        pt.y = parseLayoutMeasureVert(yaml, ys, node);
    }

    for (auto& el: yaml) {
        switch (pshHash(el.key)) {
        case "x"_psh: pt.x = parseLayoutMeasureHorz(el, node); break;
        case "y"_psh: pt.y = parseLayoutMeasureVert(el, node); break;
        default: raiseErrorInvalidKey(el);
        }
    }

    if (pt.x.valid || pt.y.valid)
        pt.valid = true;

    return pt;
}


SourceSize Loader::parseNodeSize(YamlNodeA yaml, NodeP node)
{
    SourceSize sz{};

    if (!yaml.isNull()) {
        auto [ws, hs] = str_split_trim(yaml.value, ',');
        sz.w = parseLayoutMeasureHorz(yaml, ws, node);
        sz.h = parseLayoutMeasureVert(yaml, hs, node);
    }


    for (auto& el: yaml) {
        switch (pshHash(el.key)) {
        case "width"_psh:
        case "w"_psh: sz.w = parseLayoutMeasureHorz(el, node); break;
        case "height"_psh:
        case "h"_psh: sz.h = parseLayoutMeasureVert(el, node); break;
        default: raiseErrorInvalidKey(el);
        }
    }

    if (sz.w.valid || sz.h.valid)
        sz.valid = true;

    return sz;
}


void Loader::parseSpacialValue(YamlNodeA yaml, StrView text, NodeP node, SpacialValueP value)
{
    if (text.empty()) {
        value->invalidate();
        return;
    }

    auto [refKey, refName] = str_split(text, '.');

    // Check if this is variable
    if (!ParserBase::isDigit(refKey.front()) && !refName.empty()) {
        switch (pshHash(refKey)) {
        case "var"_psh:
            if (auto var = blueprint_->variables.get(refName)) {
                //value->setVariable(var);
                parseSpacialValueText(yaml, var->value.getString(), node, value);
            } else
                raiseParseError(yaml, "Variable not found: %s", refName);
            break;
        case "node"_psh:
            ADebugTrap();
        default: raiseErrorInvalidKey(yaml);
        }

        return;
    } 

    parseSpacialValueText(yaml, text, node, value);
}


void Loader::parseSpacialValueText(YamlNodeA yaml, StrView text, NodeP node, SpacialValueP value)
{
    auto data = text.data();
    auto count = text.length();

    if (data[count-1] == '%') {
        value->units = MeasureUnit::Percent;
        --count;
    }

    verifySource(count, yaml, "Empty coordinate: %s", text);

    auto valueText = str_trim(StrView{data, count});

    auto v = parseReal<Real>(valueText);
    value->setReal(v);
}


}  // namespace mangl::blueprint
