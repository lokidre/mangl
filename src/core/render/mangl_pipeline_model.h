/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../mangl_render.h"

#include "../scene/mangl_model.h"

namespace mangl::render {

MInlineAuto pipelineRenderModel(RenderContextA ctx) noexcept
{
    auto& prim = *ctx.prim;

    RenderTextureInfo* textureInfo{};

    auto model = prim.model;

    auto lineWidth = model->lineWidth_;

    int lastMaterial{-1};
    Color ambientColor{};

    auto processMaterial = [&](int materialIndex) {
        if (materialIndex == lastMaterial)
            return;

        lastMaterial = materialIndex;

        if(materialIndex >= apl::count(model->materials)) {
            ambientColor = pipelineColors_[0];
        } else {

            auto &material = model->materials[materialIndex];

            if (material.ambient)
                ambientColor = material.ambient;
            else
                ambientColor = pipelineColors_[0];


            if (glState_.shadingEnabled) {
                setUniformStateValue(&glState_.material, &material);
            }

        }


        if (prim.alphaFlag_) {
            // We need to adjust the ambient material
            // Very inefficient. The model alpha shouldn't be set here
            // TODO: in shaders. In general all the "alpha" move to the shaders
            ambientColor.a *= prim.alpha_;
        }
    };

    if (model->wireframe) {
        setUniformStateValue(&glState_.lineWidth, lineWidth);
        disableTextures();
        disableShading();
    } else if (model->vertices) {
        setUniformStateValue(&glState_.lineWidth, lineWidth);
        disableTextures();
        disableShading();
    }

    if (glState_.texture) {
        for (auto&& group: model->groups) {
            for (auto&& face: group.faces) {
                // deal with texture
                if (!textureInfo && face.ft && prim.model->texture.tid) {
                    textureInfo = findTextureInfo(prim.model->texture.tid);
                }
            }
        }

        if (textureInfo) {
            enableTextures(textureInfo);
        } else {
            disableTextures();
        }
    }


    // determine scale
    //auto bbox = model->getBoundingBox();
    //auto maxf = (RealGL)bbox.max();
    //glState_.vtxPosScale = maxf;
    //glState_.vtxPosScaleRecip = 1 / glState_.vtxPosScale;


    auto faceVertex2RenderVertex = [&](const Model::FaceVertex &fv, RenderVertex &rv) {
        rv = {};

        rv.pos = model->vertexPositions[fv.p];

        if (textureInfo)
            rv.texture = model->vertexTextures[fv.t];

        rv.color = ambientColor;

        if (prim.alphaFlag_)
            rv.color.a *= prim.alpha_;

        if (glState_.shadingEnabled)
            rv.normal = model->vertexNormals[fv.n];
    };

    auto polyVertex2RenderVertex = [&](int fvp, RenderVertex &rv) {
        rv = {};

        rv.pos = model->vertexPositions[fvp];

        rv.color = ambientColor;

        if (prim.alphaFlag_)
            rv.color.a *= prim.alpha_;
    };


    RenderVertex v[3];

    for (auto &group: model->groups) {
        for (auto &face: group.faces) {

            if (face.v.size() < 3)
                continue;

            processMaterial(face.m);

            auto aIt = face.v.begin();
            auto bIt = aIt;
            ++bIt;
            auto cIt = bIt;
            ++cIt;
            auto vEnd = face.v.end();

            // prepare the screen coordinates
            faceVertex2RenderVertex(*aIt, v[0]);
            faceVertex2RenderVertex(*bIt, v[1]);

            if (model->wireframe) {
                beginLines(1);
                pushIndices(pushVertices(v[0], v[1]), 0, 1); // 1 line
            } else if (model->vertices) {
                beginPoints(2);
                pushIndices(pushVertices(v[0], v[1]), 0, 1); // 2 points
            }


            while (cIt != vEnd) {

                faceVertex2RenderVertex(*cIt, v[2]);

                if (model->wireframe)
                    beginLines(1);
                else if (model->vertices)
                    beginPoints(1);
                else
                    beginTriangles(1);


                if (model->wireframe)   // if wireframe
                    pushIndices(pushVertices(v[1], v[2]), 0, 1); // 1 line
                else if (model->vertices)   // if vertices
                    pushIndex(pushVertex(v[2]), 0); // point
                else
                    pushIndices(pushVertices(v[0], v[1], v[2]), 0, 1, 2); // triangle

                bIt = cIt;
                ++cIt;
                v[1] = v[2];
            }  // for polygon vertices

        }  // for all faces

        if (!group.lines.empty()) {
            auto cullingEnabled = glState_.cullFace;
            setUniformStateValue(&glState_.cullFace, false);

            for (auto& polygon: group.lines) {
                if (polygon.v.size() < 2)
                    continue;

                processMaterial(polygon.m);

                auto aIt = polygon.v.begin();
                auto bIt = aIt;
                ++bIt;
                auto vEnd = polygon.v.end();

                // prepare the screen coordinates
                polyVertex2RenderVertex(*aIt, v[0]);

                while (bIt != vEnd) {

                    polyVertex2RenderVertex(*bIt, v[1]);

                    pipelineRenderLineBody(lineWidth, v[0], v[1]);

                    aIt = bIt;
                    ++bIt;
                    v[0] = v[1];
                } // for polygon vertices
            }     // for all lines


            setUniformStateValue(&glState_.cullFace, cullingEnabled);
        }



    }  // for all groups
}

}  // namespace mangl::render
