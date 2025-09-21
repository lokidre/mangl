/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_main_window.h"

#include "../../include/mangl/audio.h"

#if MANGL_WINMAIN

namespace mangl {

int ManglMainWindow::onWinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow
) {
    int retCode = 1;

    hInstance_ = hInstance;
    hPrevInstance_ = hPrevInstance;


    auto handleException = [&](StrView message) {
        apl::ApiString text{message};
        auto flags = MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND;
        MessageBoxW(hwnd(), text.win(), L"Error!", flags);
    };


    try {
        auto windowStyle = makeMainWindowStyleFlags();

        auto wndRect = makeMainWindowRect(windowStyle);

        createMainWindow(NULL, wndRect, windowStyle);

        if (!Music::settingEnabled)
            Music::enable(false);

        if (!Sfx::settingEnabled)
            Sfx::enable(false);

        MSG msg{};

#if MANGL_WIN_GAME_LOOP_TIMER_MM

        while (GetMessageW(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

#endif

#if MANGL_WIN_GAME_LOOP_TIMER_SLEEP
        bool exitMessageLoop = false;
        while (!exitMessageLoop) {
            while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);

                //if (WM_QUIT == msg.message)
                //    mainLoopPendingExit_ = true;
            }

            if (mainLoopPendingExit_)
                exitMessageLoop = true;

            multimediaTimerCallback();
        }
#endif

        destroyMainWindow();

        // Delete all the singletons
        apl::SingletonManager::instance()->shutdown();

        retCode = 0;
    } catch (const std::exception& e) {
        handleException(e.what());
    } catch (...) {
        handleException("WinMain: Unhandled Exception");
    }

    return retCode;
}
} // namespace mangl

#endif

