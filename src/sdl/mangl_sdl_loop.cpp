/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_sdl_loop.h"

#include "mangl_sdl_main_window.h"
#include "mangl_sdl_keyboard.h"

#include "../core/main/mangl_main.h"



#if MANGL_SDL

#include "mangl_sdl.h"

#if MANGL_SDL3
#include <SDL3/SDL_opengl.h>
#else
#include <SDL2/SDL_opengl.h>
#endif


namespace mangl {

// Keyboard shortcuts support
static std::vector<KeyCode> currentPressedKeys_;
static void removePressedKey(KeyCode code) {
    std::erase(currentPressedKeys_, code);
}


//static Uint32 sdl_timer_cb(Uint32 interval, void* param) {
//    SDL_Event e{};
//    e.type = SDL_EVENT_USER;
//    SDL_PushEvent(&e);
//    return interval;
//}

static auto coordToPoint(double x, double y) noexcept {
    auto p = Point::make(x, y);

    p.y = Env::applicationRect.h - 1 - p.y;

    p /= Env::screenScale;
    return p;
}

void Framework::finish() {
    SDL_Event ev;
    ev.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&ev);
}



int sdl_main_loop() {
    ManglMainWindow* mainWindow{};
    try {
        SDL_Init(SDL_INIT_VIDEO);

        mangl_main();
        Framework::instance()->init();

        mainWindow = ManglMainWindow::instance();

        mainWindow->initMainLoop();
        mainWindow->runMainLoop();

    } catch (const std::exception& e) {
        SDL_MessageBoxFlags flags = SDL_MESSAGEBOX_ERROR;
        SDL_ShowSimpleMessageBox(flags, "Error", e.what(), mainWindow ? mainWindow->sdlWindow_ : nullptr);
        return 1;
    }

    return 0;
}

void ManglMainWindow::initMainLoop() {

    auto appRect = Env::applicationRect.convert<RectI>();

    if (appRect.w == M::InvalidCord || appRect.h == M::InvalidCord) {
        appRect.resize(Env::DefaultAppSize.convert<SizeI>());
    }

    auto screenSize = Env::physScreenSize.convert<SizeI>();
    auto windowSize = SizeI::make(appRect.w, appRect.h);

    auto desktopRect = RectI::make(0, 0, screenSize.w, screenSize.h);

    auto wndRect = desktopRect.centered(windowSize);

    if (appRect.x == M::InvalidCordI || appRect.y == M::InvalidCordI) {
        appRect.x = wndRect.x;
        appRect.y = wndRect.y;
    }

    Env::applicationRect = appRect.convert<Rect>();


    int windowFlags = SDL_WINDOW_OPENGL;
    auto title = Env::appTitle.c_str();

    PointI undefinedPos{SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED};
    auto topLeft = stateWindowTopLeft.get(undefinedPos);

#if MANGL_SDL3
    if (topLeft != undefinedPos) {
        int displaysCount = 0;
        auto displays = SDL_GetDisplays(&displaysCount);

        bool fitFound = false;
        for (auto display = displays; *display; ++display) {
            SDL_Rect displayRect{};
            SDL_GetDisplayUsableBounds(*display, &displayRect);

            if (topLeft.x < displayRect.x || topLeft.y < displayRect.y)
                continue;

            if (topLeft.x + wndRect.w > displayRect.x + displayRect.w)
                continue;

            if (topLeft.y + wndRect.h > displayRect.y + displayRect.h)
                continue;

            fitFound = true;
            break;
        }

        if (!fitFound)
            topLeft = undefinedPos;

        SDL_free(displays);
    }
#endif


#if MANGL_SDL3
    sdlWindow_ = SDL_CreateWindow(title, wndRect.w, wndRect.h, windowFlags);
    SDL_SetWindowPosition(sdlWindow_, topLeft.x, topLeft.y);
#else 
    windowFlags |= SDL_WINDOW_SHOWN;
    sdlWindow_ = SDL_CreateWindow(
        title, topLeft.x, topLeft.y, wndRect.w,
        wndRect.h, windowFlags
    );
#endif

    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    auto glContext = SDL_GL_CreateContext(sdlWindow_);

    // ----- SDL v-sync
    SDL_GL_SetSwapInterval(1);

    //SDL_AddTimer(1000 / Uint32(mainWindow->fps_), sdl_timer_cb, mainWindow);
}

void ManglMainWindow::runMainLoop() {
#if MANGL_OPENGL
    initOpenGL();
#endif

    auto fw = Framework::instance();
    auto mouseButtonDown = false;

    bool running = true;
    while (running) {
        SDL_Event ev;

        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                try {
                    switch (ev.button.button) {
                    case SDL_BUTTON_LEFT:
                        fw->onTouchDown({
                            .p = coordToPoint(ev.motion.x, ev.motion.y),
                            .timestamp = timestamp_,
                            .index = 0,
                            .count = 1,
                        });

                        mouseButtonDown = true;
                        break;
                    default:
                        break;
                    }
                } catch (const std::exception& e) {
                    mmCallbackError(e.what());
                }
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                try {
                    switch (ev.button.button) {
                    case SDL_BUTTON_LEFT:
                        fw->onTouchUp({
                            .p = coordToPoint(ev.motion.x, ev.motion.y),
                            .timestamp = timestamp_,
                            .index = 0,
                            .count = 1,
                        });

                        mouseButtonDown = false;
                        break;
                    default:
                        break;
                    }
                } catch (const std::exception& e) {
                    mmCallbackError(e.what());
                }
                break;

            case SDL_EVENT_MOUSE_MOTION:
                try {
                    switch (ev.button.button) {
                    case SDL_BUTTON_LEFT:
                        if (!mouseButtonDown)
                            break;

                        fw->onTouchMove({
                            .p = coordToPoint(ev.motion.x, ev.motion.y),
                            .timestamp = timestamp_,
                            .index = 0,
                            .count = 1,
                        });
                        break;
                    default:
                        break;
                    }
                } catch (const std::exception& e) {
                    mmCallbackError(e.what());
                }
                break;

            case SDL_EVENT_KEY_DOWN:
                try {
#if MANGL_SDL3
                    auto scanCode = ev.key.scancode;
#else
                    auto scanCode = ev.key.keysym.scancode;
#endif
                    auto it = sdlScanCodes_.find(scanCode);
                    if (it == sdlScanCodes_.end())
                        break;
                    auto code = it->second;
                    removePressedKey(code);
                    currentPressedKeys_.push_back(code);

                    fw->onKeyDown(it->second);
                } catch (const std::exception& e) {
                    mmCallbackError(e.what());
                }
                break;

            case SDL_EVENT_KEY_UP:
                try {
#if MANGL_SDL3
                    auto scanCode = ev.key.scancode;
#else
                    auto scanCode = ev.key.keysym.scancode;
#endif
                    auto it = sdlScanCodes_.find(scanCode);
                    if (it == sdlScanCodes_.end())
                        break;

                    if (currentPressedKeys_ == fw->fullScreenShortcut_) {
                        // TODO: full screen
                        //toggleFullScreen();
                    }


                    if (currentPressedKeys_ == fw->screenshotShortcut_) {
                        Framework::getRenderer()->takeScreenshot();
                    }

                    if (currentPressedKeys_ == fw->quitShortcut_) {
                        Framework::finish();
                    }

                    if (currentPressedKeys_ == fw->backShortcut_) {
                        fw->processBack();
                    }


                    auto code = it->second;

                    removePressedKey(code);

                    fw->onKeyUp(it->second);

                } catch (const std::exception& e) {
                    mmCallbackError(e.what());
                }
                break;

            case SDL_EVENT_QUIT:
                try {
                    int posX{}, posY{};
                    SDL_GetWindowPosition(sdlWindow_, &posX, &posY);
                    stateWindowTopLeft = PointI{posX, posY};

                    fw->onTerminate();
                } catch (const std::exception& e) {
                    mmCallbackError(e.what());
                }
                running = false;
                break;

            default: break;
            }
        }


        processLoopIdle();
        SDL_GL_SwapWindow(sdlWindow_);

        //        glViewport(0, 0, wndRect.w, wndRect.h);
        //        glClearColor(1.f, 0.f, 1.f, 1.f);
        //        glClear(GL_COLOR_BUFFER_BIT);
        //
        //        SDL_GL_SwapWindow(window);

    }  // Main game loop

}



} // namespace mangl

#endif // if SDL
