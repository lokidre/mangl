/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../../src/core/mangl_setup.h"

#if APEAL_OS_WINDOWS
#include "../../src/windows/mangl_win_main_window.h"
#elif APEAL_OS_MAC
#include "../../src/mac/mangl_main_window.h"
#elif MANGL_GTK
#include "../../src/gtk/mangl_gtk_main_window.h"
#elif MANGL_SDL
#include "../../src/sdl/mangl_sdl_main_window.h"
#endif

