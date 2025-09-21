/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_sdl_main_window.h"


#if MANGL_SDL && MANGL_OPENGL

#include "mangl_sdl.h"

#include "../core/opengl/mangl_opengl.h"

namespace mangl {

void ManglMainWindow::initOpenGL()
{
    auto err = glewInit();
    apl::verify(GLEW_OK == err, "GLEW Error: %s", reinterpret_cast<const char*>(glewGetErrorString(err)));
}

void ManglMainWindow::releaseOpenGL()
{
}

}  // namespace mangl

#endif
