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

namespace mangl::render {

MInlineAuto processPipelineProjection(RenderContext &ctx) noexcept
{
    auto prim = ctx.prim;

    //glState_.modelMtx = &sceneRender_->defaultModelMtx_;
    //glState_.viewMtx = &sceneRender_->defaultViewMtx_;
    //glState_.projMtx = &sceneRender_->currentProjMtx_;

    if (prim->type == PrimType::Model) {

        auto model = prim->model;

        // adjust model view matrix
        if (model->modelMatrix) {
            setUniformStateValue(&glState_.modelMtx, model->modelMatrix);
        } else {
            setUniformStateValue(&glState_.modelMtx, &sceneRender_->defaultModelMtx_);
        }

        if (model->viewMatrix) {
            setUniformStateValue(&glState_.viewMtx, model->viewMatrix);
        } else {
            setUniformStateValue(&glState_.viewMtx, &sceneRender_->defaultViewMtx_);
        }

        if (model->projMatrix) {
            setUniformStateValue(&glState_.projMtx, model->projMatrix);
        } else {
            setUniformStateValue(&glState_.projMtx, &sceneRender_->currentProjMtx_);
        }

        return;
    }

    if (prim->modelMtx) {
        setUniformStateValue(&glState_.modelMtx, prim->modelMtx);
    } else {
        setUniformStateValue(&glState_.modelMtx, &sceneRender_->defaultModelMtx_);
    }

    setUniformStateValue(&glState_.viewMtx, &sceneRender_->defaultViewMtx_);
    setUniformStateValue(&glState_.projMtx, &sceneRender_->currentProjMtx_);

}


}  // namespace mangl::opengl

