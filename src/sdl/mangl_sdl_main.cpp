/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "../core/mangl_setup.h"

#if defined(MANGL_SDL) && !defined(MANGL_NO_MAIN)

#if MANGL_SDL3
#include <SDL3/SDL_main.h>
#endif

#include "mangl_sdl.h"
#include "mangl_sdl_loop.h"


int main(int argc, char* argv[]) { 
    SDL_SetMainReady();
    return mangl::sdl_main_loop(); 
}

#endif // SDL && main
