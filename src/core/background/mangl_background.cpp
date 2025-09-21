/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_background.h"

#include "../mangl_framework.h"

#include <cognit/func.hpp>

namespace mangl {

void MBackgroundLayer::clear()
{
    textures.clear();

    valid = false;
    index = 0;

    period.clear();
    speed.clear();
    direction.clear();
    startXPercent = 0;
    single = false;

    foreground = false;

    baseline.clear();
    alpha.clear();
    position.clear();
    static_flag = false;

    flip_horz = false;
    tile = false;
    full_screen = false;

    pattern = false;

    currentExtent = {};
    appear_timestamp = 0;
    elementIndexBegin = elementIndexEnd = currentElementIndex = 0;

    alignHorz = M::AlignLeft, alignVert = M::AlignBottom;
}


Real MBackground::variation(Real value)
{
    Real v{};

    return M::random.range(value - value/8, value + value/8, &v);
}



//
// Setup
//
void MBackground::setSprites(Prim *sprites, int count)
{
    prims = sprites;

    auto sptr = sprites;
    elements_.resize(count);
    for (auto &el : elements_) {
        el.so = sptr;
        ++sptr;
    }
}

void MBackground::clear()
{
    speedUnits_ = PARAM_SCALE;
    backgroundColorFlag_ = false;
    started_ = false;
    direction_ = {-1, 0};
    gradients_.clear();

    layers_.clear();
}


Real MBackground::computeValueComponent(const MBackgroundParameter &param, Real value)
{
    if (!param.set)
        return 0;

    const auto &alongExtent = direction_.y == 0 ? viewport_.w : viewport_.h;
    const auto &crossExtent = direction_.y == 0 ? viewport_.h : viewport_.w;

    switch (param.alongDirection) {
    case PARAM_ALONG: return value * 0.01 * alongExtent;
    case PARAM_CROSS: return value * 0.01 * crossExtent;
    case PARAM_DIAGONAL: return value * 0.01 * diagonal_;
    default: break;
    }

    return value;
}

Real MBackground::value(const MBackgroundParameter &param)
{
    if (!param.set)
        return 0;

    Real value;

    if (param.isRandom) {
        value = random(computeValueComponent(param, param.minValue), computeValueComponent(param, param.maxValue));
    } else {
        value = computeValueComponent(param, param.value);
    }

    return value;
}

Real MBackground::skewedValueExp(const MBackgroundParameter &param, Real coeff)
{
    if (!param.set)
        return 0;

    Real value;

    if (param.isRandom) {
        Real rangeMin = computeValueComponent(param, param.minValue);
        Real rangeMax = computeValueComponent(param, param.maxValue);

        Real range = rangeMax-rangeMin;

        value = random(range * coeff);

        Real f = value/range;
        value = exp(-f*f) * 0.36787944117;
        value *= range * coeff;
        value += rangeMin;

    } else {
        value = computeValueComponent(param, param.value);
    }

    return value;
}


Real MBackground::minValue(const MBackgroundParameter &param)
{
    return computeValueComponent(param, param.isRandom ? param.minValue : param.value);
}

Real MBackground::maxValue(const MBackgroundParameter &param)
{
    return computeValueComponent(param, param.isRandom ? param.maxValue : param.value);
}

}  // namespace mangl
