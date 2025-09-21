/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_sdl_main_window.h"

#if MANGL_SDL

#include "mangl_sdl.h"


namespace mangl {

ManglMainWindow::ManglMainWindow()
{
    //fps_ = 30;
    fps_ = 60;

#ifdef MANGL_FPS
    fps_ = MANGL_FPS;
#endif

    fpsRecipTimer_ = static_cast<uint64_t>(static_cast<Real>(1000) / fps_);
}

ManglMainWindow::~ManglMainWindow() = default;

}  // namespace mangl

#endif
