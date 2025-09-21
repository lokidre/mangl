/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_main_window.h"
#include "../core/main/mangl_main.h"

#if MANGL_WINMAIN

#if MANGL_CONSOLE || MANGL_NO_MAIN
#else

int WINAPI WinMain(_In_ HINSTANCE inst, _In_opt_ HINSTANCE prev, _In_ LPSTR cmdline, _In_ int show) {
    using namespace mangl;
    mangl_main();
    Framework::instance()->init();
    return ManglMainWindow::instance()->onWinMain(inst, prev, cmdline, show);
}
#endif

#endif  // Winmain

