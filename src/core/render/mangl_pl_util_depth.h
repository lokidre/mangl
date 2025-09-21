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

namespace mangl::render {

MInlineAuto processDepthTest(RenderContext &ctx) noexcept
{
    auto model = ctx.prim->model;

    if (!model) {
        setUniformStateValue(&glState_.depthTest, false);
        setUniformStateValue(&glState_.cullFace, false);

        //glState_.depthTest = 0;
        //glState_.cullFace = 0;
        return;
    }

    setUniformStateValue(&glState_.depthTest, model->depthTest);
    setUniformStateValue(&glState_.cullFace, model->faceCulling);
    //glState_.depthTest = model->depthTest;
    //glState_.cullFace = model->faceCulling;
}


}  // namespace mangl::render
