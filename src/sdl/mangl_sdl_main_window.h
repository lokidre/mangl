/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_types.h"
#include "../core/mangl_framework.h"
#include "../core/mangl_storage.h"

struct SDL_Window;

namespace mangl {

class ManglMainWindow: public apl::Singleton<ManglMainWindow, false> {
    Stat<PointI> stateWindowTopLeft{"mangl.sdl.main_window.top_left"};
    Stat<SizeI> stateWindowSize{"mangl.sdl.main_window.size"};
    Stat<bool> stateWindowMaximized{"mangl.sdl.main_window.maximized"};
    Stat<bool> stateWindowFullScreen{"mangl.sdl.main_window.fullscreen"};

public:
    SDL_Window* sdlWindow_{};

    Timestamp fps_{};
    uint64_t fpsRecipTimer_{};  // 1 / FPS in milliseconds

    void destroy() {}
    bool exitOnClose_ = false;

    bool gameLoopNeedExit_{};
    bool timerInitialized_{};
    Timestamp timestamp_{};

    ManglMainWindow();
    ~ManglMainWindow() override;

#if MANGL_OPENGL
    void initOpenGL();
    void releaseOpenGL();
#endif

// Audio MCI support
#if MANGL_MUSIC_MCI
    friend class MusicEngineMCI;
    MusicEngineMCI* mciDevice_{};
#endif


    bool processLoopIdle();
    void initMainLoop();
    void runMainLoop();
    bool onTimer();

private:
    void mmCallbackError(const char* err);
};

} // namespace mangl
