/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_model.h"

#include <cognit/mtx.hpp>
#include <cognit/vec.hpp>

#include <algorithm>

namespace mangl {


Model &Model::clear()
{
    materials.clear();
    groups.clear();
    vertexPositionsOrig.clear();
    vertexTexturesOrig.clear();
    vertexNormalsOrig.clear();
    vertexPositions.clear();
    vertexTextures.clear();
    vertexNormals.clear();
    boundingBoxCalculated_ = false;

    texture.tid = 0;
    texture.coords = nullptr;
    texture.count = 1;

    rotationRho_ = rotationTheta_ = rotationKsi_ = 0;

    return *this;
}

Model &Model::normalize(bool center)
{
    if (vertexPositions.size() < 2)
        return *this;

    auto bbox = calculateBoundingBox(vertexPositions);

    /*
    Point3D mins(vertexPositions[0]), maxs(vertexPositions[0]) ;

    for ( auto &v: vertexPositions ) {
        for ( int i = 0 ; i < 3 ; ++i ) {
            if ( v[i] < mins[i] ) mins[i] = v[i] ;
            if ( v[i] > maxs[i] ) maxs[i] = v[i] ;
        }
    }

    Cord scale = 0 ;
    for ( int i = 0 ; i < 3 ; ++i ) {
        if ( maxs[i] - mins[i] > scale )
            scale = maxs[i] - mins[i] ;
    }
    */

    auto scale = (RealGL)std::max(std::max(bbox.w, bbox.h), bbox.d);

    if (center) {
        auto centroid = bbox.centroid().convert<Point3DGL>();
        auto p = vertexPositions.data();
        auto total = vertexPositions.size();
        for (auto i = 0; i < total; ++i, ++p)
            p->sub(centroid);
    }

    if (cognit::tiny(scale))
        return *this;

    auto s = 1/scale;
    auto p = static_cast<VertexPosition *>(vertexPositions.data());
    auto total = vertexPositions.size();
    for (auto i = 0; i < total; ++i, ++p)
        p->scale(s);
    //for ( auto &v: vertexPositions )
    //	v.scale(1/scale) ;

    boundingBoxCalculated_ = false;

    return *this;
}

Model &Model::scale(Cord s)
{
    auto scale = static_cast<RealGL>(s);

    for (auto& v: vertexPositions) {
        v.scale(scale);
    }

    boundingBoxCalculated_ = false;

    return *this;
}

Box3D Model::calculateBoundingBox(const VertexVector &v)
{
    Box3D bb{};
    auto *p = v.data();
    auto total = v.size();

    bb.x = p->x, bb.y = p->y, bb.z = p->z;

    for (auto i = 1; i < total; ++i, ++p) {
        if (p->x < bb.x) bb.w = bb.x + bb.w - p->x, bb.x = p->x;
        if (p->y < bb.y) bb.h = bb.y + bb.h - p->y, bb.y = p->y;
        if (p->z < bb.z) bb.d = bb.z + bb.d - p->z, bb.z = p->z;

        if (p->x >= bb.x + bb.w) bb.w = p->x - bb.x;
        if (p->y >= bb.y + bb.h) bb.h = p->y - bb.y;
        if (p->z >= bb.z + bb.d) bb.d = p->z - bb.z;
    }


    return bb;
}

Box3D Model::calculateBoundingBox()
{
    boundingBox_ = calculateBoundingBox(vertexPositions);
    boundingBoxCalculated_ = true;
    return boundingBox_;
}


Model &Model::sync(const Model &m)
{
    if (vertexPositions.size() != m.vertexPositions.size() || vertexNormals.size() != m.vertexNormals.size())
        throw std::runtime_error("Models don't match");

    auto mv = m.vertexPositions.begin();
    for (auto& v: vertexPositions)
        v = *mv++;

    auto mn = m.vertexNormals.begin();
    for (auto& n: vertexNormals)
        n = *mn++;

    return *this;
}

Model &Model::generateNormals()
{
    using namespace cognit;

    auto generateNormal = [this](auto& face, Index uIndex, Index aIndex, Index bIndex) {
        auto &uv = face.v[uIndex], &av = face.v[aIndex], &bv = face.v[bIndex];
        auto a = vertexPositions[av.p] - vertexPositions[uv.p];
        auto b = vertexPositions[bv.p] - vertexPositions[uv.p];
        vertexNormals[uv.n] = apl::cross(a, b); //vct_cross ( a, b, vertexNormals[uv.n] ) ;
        vertexNormals[uv.n].normalize();
    };


    Index normalIndex{};

    for (auto &group: groups) {
        for (auto &face: group.faces) {
            if (normalIndex + face.v.size() >= vertexNormals.size())
                vertexNormals.resize(normalIndex+face.v.size());
            for (auto &v: face.v)
                v.n = (int)normalIndex++;
            face.fn = true;


            //generateNormal(0,2,1) ;
            //generateNormal(1,0,2) ;
            //generateNormal(2,1,0) ;
            generateNormal(face, 0, 1, 2);
            generateNormal(face, 1, 2, 0);
            //generateNormal(2,0,1) ;

            auto vIt = face.v.begin(), vEnd = face.v.end();
            ++vIt, ++vIt;
            int vIdx = 2;

            while (vIt != vEnd) {
                generateNormal(face, vIdx, 0, vIdx-1);
                ++vIt;
                ++vIdx;
            }
        }
    }
    //verticeNormals.resize(verticePositions.size()) ;

    normalsGenerated_ = true;

    return *this;
}


Point Model::getProjection(Point p)
{
    using namespace cognit;

    Point projection{};

    return projection;
}


}  // namespace mangl
