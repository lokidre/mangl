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


MInlineAuto processFog(RenderContext& ctx) noexcept
{
    // TODO: make per model per prim
    Fog* fog{};

    if (auto model = ctx.prim->model; model && model->fog)
        fog = model->fog;

    if (fog && fog->enabled) {
        //setUniformStateValue(&glState_.fogEnabled, true);
        setUniformStateValue(&glState_.fog, fog);

        //glState_.fogEnabled = true;
        //glState_.fog = fog;
    } else {
        setUniformStateValue(&glState_.fog, nullptr);
        //setUniformStateValue(&glState_.fogEnabled, false);
        //glState_.fogEnabled = false;
    }
}

}  // namespace mangl::render
