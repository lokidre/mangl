/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_and_main_engine.h"

#include <swappy/swappyGL.h>

namespace mangl {

void AndroidMainEngine::initOpenGL()
{
    renderInitDisplay();
    renderInitConfig();
    renderInitSurface();
    renderInitContext();
}

void AndroidMainEngine::destroyOpenGL()
{
    renderDestroyContext();
    renderDestroySurface();
    renderDestroyDisplay();
}


//void GraphicsAPIGLES::QueryAvailability() {
//    EGLNativeWindowType nativeWindow = PlatformUtilGLES::GetNativeWindow();
//    if (nativeWindow != nullptr) {
//        QueryCapabilities();
//    }
//}
//

void AndroidMainEngine::renderInitDisplay()
{
    MDebugFunc();

    if (eglDisplay_ != EGL_NO_DISPLAY)
        return;

    eglDisplay_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint versionMajor{}, versionMinor{};

    auto result = eglInitialize(eglDisplay_, &versionMajor, &versionMinor);
    apl::verify(result == EGL_TRUE, "Error:eglInitialize: %d", eglGetError());

    MDebugPrint("eglInitialize: %d.%d", versionMajor, versionMinor);
}

void AndroidMainEngine::renderInitConfig()
{
    MDebugFunc();

    EGLint numConfigs;
    const EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // request OpenGL ES 2.0
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,

        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_NONE
    };

    auto result = eglChooseConfig(eglDisplay_, attribs, &eglConfig_, 1, &numConfigs);
    apl::verify(result, "eglChooseConfig fail");
}



void AndroidMainEngine::renderInitSurface() {
    MDebugFunc();

    if (eglSurface_ != EGL_NO_SURFACE)
        return;

    eglSurface_ = eglCreateWindowSurface(eglDisplay_, eglConfig_, Jni::app->window, nullptr);

    apl::verify(EGL_NO_SURFACE != eglSurface_, "eglCreateWindowSurface fail");
}

void AndroidMainEngine::renderInitContext() {
    MDebugFunc();

    if (eglContext_ != EGL_NO_CONTEXT)
        return;

    if (eglDisplay_ == EGL_NO_DISPLAY || eglConfig_ == nullptr)
        return;

    const EGLint contextAttribs3[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };

    const EGLint contextAttribs2[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    eglContext_ = eglCreateContext(eglDisplay_, eglConfig_, nullptr, contextAttribs3);
    if (EGL_NO_CONTEXT == eglContext_) {
        eglContext_ = eglCreateContext(eglDisplay_, eglConfig_, nullptr, contextAttribs2);
    }
    apl::verify(EGL_NO_CONTEXT != eglContext_, "eglCreateContext");



    auto res = eglMakeCurrent(eglDisplay_, eglSurface_, eglSurface_, eglContext_);
    apl::verify(res, "eglMakeCurrent");
}

void AndroidMainEngine::renderDestroyContext() {
    MDebugFunc();
    if (eglContext_ == EGL_NO_CONTEXT)
        return;

    eglMakeCurrent(eglDisplay_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglContext_ = EGL_NO_CONTEXT;
}

void AndroidMainEngine::renderDestroySurface() {
    MDebugFunc();

    //renderDestroyContext();

    if (eglSurface_ == EGL_NO_SURFACE)
        return;

    eglMakeCurrent(eglDisplay_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(eglDisplay_, eglSurface_);

    eglSurface_ = EGL_NO_SURFACE;
}


void AndroidMainEngine::renderDestroyDisplay() {
    MDebugFunc();

    if (eglDisplay_ == EGL_NO_DISPLAY)
        return;

    eglTerminate(eglDisplay_);
    eglDisplay_ = EGL_NO_DISPLAY;
}


//
//
//bool AndroidMainEngine::unitializeGraphicsAPI() {
//    eglDisplay_ = InitializeEGLDisplay();
//
//    if (eglDisplay_ != EGL_NO_DISPLAY) {
//        eglInitialize(eglDisplay_, nullptr, nullptr);
//
//        // if (PlatformUtilGLES::CheckEGLError("Calling eglInitialize")) {
//        //     api_status_ = kGraphicsAPI_Active;
//        //     return true;
//        // }
//    }
//    return false;
//}



}  // namespace mangl

