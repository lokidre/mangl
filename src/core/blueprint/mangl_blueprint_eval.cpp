/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
﻿#include "mangl_blueprint.h"

namespace mangl::blueprint {

Real Engine::evalCoord(SpacialValueP v, CordRange rng) noexcept
{
    if (!v->valid)
        return 0;

    auto rv = v->getReal();
    if (MeasureUnit::Percent == v->units)
        return rng.mn + rng.mx * rv / 100;
    return rv;
}


/*
Real Engine::evalX(LayoutMeasureXA lv) noexcept
{
    if (!lv.valid)
        return {};
    //if (!lv.evalReal)
    //    lv.setEvalReal(lv.getReal());
    auto realValue = lv.getReal();
    if (MeasureUnit::Percent == lv.unit)
        return viewport_.w * realValue / 100;
    return realValue;
}

Real Engine::evalY(LayoutMeasureYA lv) noexcept
{
    if (!lv.valid)
        return {};
    auto realValue = lv.getReal();
    if (MeasureUnit::Percent == lv.unit)
        return viewport_.h * realValue / 100;
    return realValue;
}
*/

void Engine::evalNodeRect(NodeP node) noexcept
{
    if (node->evalRect.evaluated)
        return;

    node->evalRect = evalRect(&node->position, &node->size);

    auto originRect = viewport_;

    if (node->origin.align & M::AlignCenter)
        node->evalRect.value.x += originRect.center().x;
    if (node->origin.align & M::AlignRight)
        node->evalRect.value.x += originRect.right();

    if (node->origin.align & M::AlignVCenter)
        node->evalRect.value.y += originRect.center().y;
    if (node->origin.align & M::AlignTop)
        node->evalRect.value.y += originRect.top();


    for (auto parent = node->parent; parent; parent = parent->parent) {
        evalNodeRect(parent);
        node->evalRect.value.x += parent->evalRect.value.x;
        node->evalRect.value.y += parent->evalRect.value.y;
    }

    if (node->align & M::AlignCenter)
        node->evalRect.value.x -= node->evalRect.value.w/2;
    if (node->align & M::AlignRight)
        node->evalRect.value.x -= node->evalRect.value.w;
    if (node->align & M::AlignVCenter)
        node->evalRect.value.y -= node->evalRect.value.h/2;
    if (node->align & M::AlignTop)
        node->evalRect.value.y -= node->evalRect.value.h;

}

void Engine::evalNodeRectRatioed(NodeP node, SizeA ratio) noexcept
{
    if (node->evalRect.evaluated)
        return;

    if (!node->size.valid && node->texture) {
        auto sz = node->texture.size() / node->texture.density;
        node->size.w.setReal(sz.w);
        //node->size.w.valid = true;
        node->size.h.setReal(sz.h);
        //node->size.h.valid = true;
        node->size.valid = true;
    }

    if (!node->size.h.valid) {
        node->size.h.setReal(evalX(&node->size.w) * ratio.h / ratio.w);
        //node->size.h.valid = true;
    }

    if (!node->size.w.valid) {
        node->size.w.setReal(evalY(&node->size.h) * ratio.w / ratio.h);
        //node->size.w.valid = true;
    }

    evalNodeRect(node);
}

}  // namespace mangl::blueprint
