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

MInlineAuto enableShading() noexcept
{
    setUniformStateValue(&glState_.shadingEnabled, true);

    //if (!glState_.shadingEnabled) {
    //    bufferFlushPrimitive();
    //    glState_.shadingEnabled = true;
    //}
}


MInlineAuto disableShading() noexcept
{
    setUniformStateValue(&glState_.shadingEnabled, false);

    //if (glState_.shadingEnabled) {
    //    bufferFlushPrimitive();
    //    glState_.shadingEnabled = false;
    //}
}

MInlineAuto processShading(RenderContext &ctx) noexcept
{
    auto model = ctx.prim->model;

    if (!model) {
        setUniformStateValue(&glState_.shadingEnabled, false);
        //glState_.shadingEnabled = false;
        return;
    }

    if (!model->lightingEnabled) {
        setUniformStateValue(&glState_.shadingEnabled, false);
        //glState_.shadingEnabled = false;
        return;
    }

    if (model->wireframe || model->vertices) {
        setUniformStateValue(&glState_.shadingEnabled, false);
        //glState_.shadingEnabled = false;
        return;
    }

    setUniformStateValue(&glState_.shadingEnabled, true);
    setUniformStateValue(&glState_.lightPos, model->lightPosition);

    //glState_.shadingEnabled = true;
    //glState_.lightPos = model->lightPosition;
}


}  // namespace 

