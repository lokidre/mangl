/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_wasm_app.h"
#include "mangl_wasm_keyboard.h"
#include "mangl_wasm_main_window.h"
#include "mangl_wasm_loop.h"

#include <emscripten/html5.h>
#include <emscripten/bind.h>


#include <iostream>



// void ManglApplication::initialize()
// {
//     std::cout << "Initializing ManglApplication." << std::endl;
// }

namespace mangl {

using LockGuard = MFrameworkNative::TimerLockGuard;


static void onWasmInitialize() {
    ManglMainWindow::instance()->onInit();
}

static void onWasmStart() {
    ManglMainWindow::instance()->onStart();
}


static void onWasmStop()
{
    std::cout << "Mangl::stop()" << std::endl;
    emscripten_pause_main_loop();
}

static void onWasmDestroy()
{
    std::cout << "Mangl::destroy()" << std::endl;
    emscripten_cancel_main_loop();
}

static auto coordToPoint(double x, double y) noexcept {
    auto p = Point::make(x, y);

//    p.y = Env::applicationRect.h - 1 - p.y;
//
//    p /= Env::screenScale;
    return p;
}

static void onWasmMouseDown(double x, double y) {
    //std::cout << "Mangl::mouseDown: " << x << ", " << y << std::endl;

    auto& mutex = MFrameworkNative::getTimerMutex();
    auto fw = MFrameworkNative::instance();
    auto mainWindow = ManglMainWindow::instance();

    TouchesInfo touch{
        1,
        {
            coordToPoint(x, y),
            mainWindow->timestamp_,
        }};

    LockGuard lock(mutex);
    fw->onTouchesBegin(touch);


}

static void onWasmMouseUp(double x, double y) {
    //std::cout << "Mangl::mouseUp: " << x << ", " << y << std::endl;

    auto& mutex = MFrameworkNative::getTimerMutex();
    auto fw = MFrameworkNative::instance();
    auto mainWindow = ManglMainWindow::instance();

    TouchesInfo touch{
        1,
        {
            coordToPoint(x, y),
            mainWindow->timestamp_,
        }};

    LockGuard lock(mutex);
    fw->onTouchesEnd(touch);

}

static void onWasmMouseMove(double x, double y) {
    //std::cout << "Mangl::mouseMove: " << x << ", " << y << std::endl;

    TouchesInfo touch{
        1,
        {
            coordToPoint(x, y),
            ManglMainWindow::instance()->timestamp_,
        }};

    LockGuard lock(MFrameworkNative::getTimerMutex());
    MFrameworkNative::instance()->onTouchesMove(touch);
}

static void onWasmMouseWheel(Cord delta, Flags keys, double x, double y) {
    std::cout << "Mangl::mouseWheel: " << delta << ", " << x << y << std::endl;

    LockGuard lock(MFrameworkNative::getTimerMutex());
    MFrameworkNative::instance()->onMouseWheel(delta, keys, coordToPoint(x, y));
}


static void onWasmMouseLeave(double x, double y) {
    //std::cout << "Mangl::mouseLeave: " << x << ", " << y << std::endl;
}

static void onKeyDown(int code) {
    std::cout << "Mangl::keyDown: " << code << std::endl;

    auto it = wasmKeyScanCodes_.find(code);
    if (it == wasmKeyScanCodes_.end())
        return;

    LockGuard lock(MFrameworkNative::getTimerMutex());
    MFrameworkNative::instance()->onKeyDown(it->second);
}

static void onKeyUp(int code) {
    std::cout << "Mangl::keyUp: " << code << std::endl;

    auto it = wasmKeyScanCodes_.find(code);
    if (it == wasmKeyScanCodes_.end())
        return;

    LockGuard lock(MFrameworkNative::getTimerMutex());
    MFrameworkNative::instance()->onKeyUp(it->second);
}


static std::string onCall(std::string name, std::string data) {
    std::cout << "Mangl::call: " << name << ": " << data << std::endl;

    LockGuard lock(MFrameworkNative::getTimerMutex());
    return MFrameworkNative::instance()->processCall(name, data);
}

}  // namespace mangl



EMSCRIPTEN_BINDINGS(Mangl) {
    emscripten::function("initialize", &mangl::onWasmInitialize);
    emscripten::function("start", &mangl::onWasmStart);
    emscripten::function("stop", &mangl::onWasmStop);
    emscripten::function("destroy", &mangl::onWasmDestroy);

    emscripten::function("call", &mangl::onCall);

    emscripten::function("onMouseDown", &mangl::onWasmMouseDown);
    emscripten::function("onMouseUp", &mangl::onWasmMouseUp);
    emscripten::function("onMouseMove", &mangl::onWasmMouseMove);
    emscripten::function("onMouseLeave", &mangl::onWasmMouseLeave);

    emscripten::function("onMouseWheel", &mangl::onWasmMouseWheel);

    emscripten::function("onKeyDown", &mangl::onKeyDown);
    emscripten::function("onKeyUp", &mangl::onKeyUp);


}

