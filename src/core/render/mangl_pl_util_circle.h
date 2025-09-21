/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "mangl_pl_util_global.h"
#include "mangl_pl_util_draw.h"

namespace mangl::render {

static
void pipelineRenderCircleBody(Prim &prim, bool filled, PointGL center, ColorA circleColor, ColorA centerColor, RealGL radius, RealGL width, M::Orientation beginSegment, M::Orientation endSegment) noexcept
{
    const auto &a = center;
    
    using std::cos;
    using std::sin;

    // for very small circle just make 2 triangles
    if (radius < 2) {
        beginTriangles(2);

        auto base = pushVertices(
            {.pos = {.x = a.x - radius, .y = a.y - radius}, .color = circleColor},
            {.pos = {.x = a.x + radius, .y = a.y - radius}, .color = circleColor},
            {.pos = {.x = a.x + radius, .y = a.y + radius}, .color = circleColor},
            {.pos = {.x = a.x - radius, .y = a.y + radius}, .color = circleColor}
        );

        pushIndex(base, 0, 1, 3);
        pushIndex(base, 3, 1, 2);

        return;
    }


    // Determine the circumference 2 * pi * r
    auto d = piGL_ * radius * 2;

    // how many pixels per segment
    constexpr auto pix = 6_mr;

    int segments = int(d / pix);

    // make each quarter dividable by 4, meaning the circle dividable by 16
    if (segments % 16 != 0)
        segments = (segments / 16 + 1) * 16;


    auto theta = piGL_ * 2 / segments;  // rotation angle

    if (prim.rtMeasure_ != theta) {
        prim.rtData1_ = cos(theta), prim.rtData2_ = sin(theta);
        prim.rtMeasure_ = theta;
    }

    auto cost = prim.rtData1_, sint = prim.rtData2_;


    // build rotation matrix
    auto R00 = cost, R01 = -sint, R10 = sint, R11 = cost;

    RenderVertex cv{.pos = {.x = a.x, .y = a.y}, .color = centerColor};  // center vertex
    RenderVertex fv{.color = circleColor};  // very first vertex
    auto fvi = fv; // first second row vertex
    auto lv = fv;  // last vertex
    auto lvi = fv;  // last second row vertex

    // adjust the segments count
    switch (beginSegment) {
    case M::Orientation0:
        fv.pos = {.x = a.x, .y = a.y - radius};
        fvi.pos = {.x = a.x, .y = a.y - (radius-width)};
        break;
    case M::Orientation90:
        fv.pos = {.x = a.x + radius, .y = a.y};
        fvi.pos = {.x = a.x + (radius-width), .y = a.y};
        break;
    case M::Orientation180:
        fv.pos = {.x = a.x, .y = a.y + radius};
        fvi.pos = {.x = a.x, .y = a.y + (radius-width)};
        break;
    case M::Orientation270:
        fv.pos = {.x = a.x - radius, .y = a.y};
        fvi.pos = {.x = a.x - (radius-width), .y = a.y};
        break;
    }

    switch (endSegment) {
    case M::Orientation0:
        lv.pos = {.x = a.x, .y = a.y - radius};
        lvi.pos = {.x = a.x, .y = a.y - (radius-width)};
        break;
    case M::Orientation90:
        lv.pos = {.x = a.x + radius, .y = a.y};
        lvi.pos = {.x = a.x + (radius-width), .y = a.y};
        break;
    case M::Orientation180:
        lv.pos = {.x = a.x, .y = a.y + radius};
        lvi.pos = {.x = a.x, .y = a.y + (radius-width)};
        break;
    case M::Orientation270:
        lv.pos = {.x = a.x - radius, .y = a.y};
        lvi.pos = {.x = a.x - (radius-width), .y = a.y};
        break;
    }

    if (beginSegment || endSegment) {
        int quartsCount = 0;
        if (endSegment < beginSegment)
            quartsCount = 4 - (beginSegment-endSegment);
        else
            quartsCount = endSegment - beginSegment;

        segments = segments / 4 * quartsCount;
    }


    auto iv = fv;  // current vertex
    auto pv = fv;  // previous vertex

    //int base = vertexBufferIndex_ ;

    int triangles = segments;

    if (!filled)
        triangles *= 2;

    beginTriangles(triangles);


    if (filled) {
        int count = segments - 1;

        // Start with center and first vertices
        auto base = pushVertices(cv, fv);

        for (int i = 0; i < count; ++i) {
            // rotate last vector
            auto vx = pv.pos.x - a.x, vy = pv.pos.y - a.y;
            iv.pos.x = R00 * vx + R01 * vy + a.x;
            iv.pos.y = R10 * vx + R11 * vy + a.y;

            pushVertex(iv);
            pv.pos = iv.pos;
        }
        if (beginSegment != endSegment)
            pushVertex(lv);


        int icv = 0;  // center vertex index

        // we last vertex is same is first vertex, we dont' duplicate it
        int ifv = 1;  // first vertex index
        int ipv{ifv};  // previous vertex index
        int ilv{ifv};  // last vertex index


        for (int i = 0; i < count; ++i, ++ipv)
            pushIndices(base, icv, ipv, ipv+1);

        if (beginSegment != endSegment)
            ilv = ipv + 1;

        pushIndex(base, icv, ipv, ilv);

    } else {
        auto ivi = fvi;  // current vertex
        auto pvi = fvi;  // previous vertex

        int count = segments - 1;

        // First and next indices
        auto base = pushVertices(fv, fvi);

        for (int i = 0; i < count; ++i) {
            // rotate last vector
            auto vx = pv.pos.x - a.x, vy = pv.pos.y - a.y;
            iv.pos.x = R00 * vx + R01 * vy + a.x;
            iv.pos.y = R10 * vx + R11 * vy + a.y;

            vx = pvi.pos.x - a.x, vy = pvi.pos.y - a.y;
            ivi.pos.x = R00 * vx + R01 * vy + a.x;
            ivi.pos.y = R10 * vx + R11 * vy + a.y;

            pushVertex(iv);
            pushVertex(ivi);
            pv.pos = iv.pos;
            pvi.pos = ivi.pos;
        }

        if (beginSegment != endSegment) {
            pushVertex(lv);
            pushVertex(lvi);
        }


        int ipv = 0;  // previous vertex index
        int ipvi = 1;  // previous vertex index - second row

        for (int i = 0; i < count; ++i) {

            pushIndices(base, ipv, ipv + 2, ipvi);
            pushIndices(base, ipvi, ipv + 2, ipvi + 2);

            ipv += 2;
            ipvi += 2;
        }

        int ilv = 0, ilvi = 1;

        if (beginSegment != endSegment)
            ilv = ipv + 2, ilvi = ipvi + 2;

        pushIndex(base, ipv, ilv, ipvi);
        pushIndex(base, ipvi, ilv, ilvi);

    }

}

// TODO: use "CircleParam" and throw everyting in there

// One color
MInlineAuto pipelineRenderCircleBody(Prim &prim, bool filled, PointGL center, ColorA color, RealGL radius, RealGL width, M::Orientation beginSegment, M::Orientation endSegment) noexcept
{
    pipelineRenderCircleBody(prim, filled, center, color, color, radius, width, beginSegment, endSegment);
}


// Version without color: get from the pipeline
MInlineAuto pipelineRenderCircleBody(Prim &prim, bool filled, PointGL center, RealGL radius, RealGL width, M::Orientation beginSegment, M::Orientation endSegment) noexcept
{
    pipelineRenderCircleBody(prim, filled, center, pipelineColors_[0], pipelineColors_[1], radius, width, beginSegment, endSegment);
}


}  // mangl::render
