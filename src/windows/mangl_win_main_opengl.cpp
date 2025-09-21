/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_main_window.h"

#if MANGL_WINMAIN && MANGL_OPENGL

#include "../core/opengl/mangl_opengl.h"

#include "../core/mangl_framework.h"

namespace mangl {

PIXELFORMATDESCRIPTOR ManglMainWindow::initOpenGLPixelDescriptor() {
    PIXELFORMATDESCRIPTOR pfd{};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    return pfd;
}


void ManglMainWindow::initOpenGL() {

    //
    // Initialize GLEW
    //

    // Create a dummy window for glew
    auto dx = CW_USEDEFAULT;
    auto dummyHWND = CreateWindowExW(0, L"Button", L"", WS_POPUP, dx, dx, dx, dx, 0, 0, 0, 0);
    auto dummyDC = GetDC(dummyHWND);


    auto pfd = initOpenGLPixelDescriptor();
    auto pixelFormat = ChoosePixelFormat(dummyDC, &pfd);
    SetPixelFormat(dummyDC, pixelFormat, &pfd);

    auto dummyHRC = wglCreateContext(dummyDC);
    wglMakeCurrent(dummyDC, dummyHRC);

    auto err = glewInit();
    apl::verify(GLEW_OK == err, "GLEW Error: %s", (const char*)glewGetErrorString(err));

    // Destroy the dummy stuff
    wglMakeCurrent(0, 0);
    wglDeleteContext(dummyHRC);
    ReleaseDC(dummyHWND, dummyDC);

    DestroyWindow(dummyHWND);

    // auto render = mangl_framework_->renderer();


    createOpenGLContext();
}

void ManglMainWindow::createOpenGLContext() {
    renderWindowDC_ = GetDC(renderHwnd_);


    if (WGLEW_EXT_swap_control)
        wglSwapIntervalEXT(true);

    if (WGLEW_ARB_pixel_format) {
        std::vector<int> defaultAttributes{
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB,     24,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     16,
            WGL_STENCIL_BITS_ARB,   0,
            WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
        };

        std::vector<int> iAttributes = defaultAttributes;
        iAttributes.insert(iAttributes.end(), {0, 0});

        FLOAT fAttributes[] = {0, 0};
        UINT numFormats{};
        int emptyPixelFormat{};
        wglChoosePixelFormatARB(renderWindowDC_, 
                                &iAttributes[0], 
                                fAttributes, 
                                1, 
                                &emptyPixelFormat, 
                                &numFormats);
    }

    auto pfd = initOpenGLPixelDescriptor();
    auto pixelFormat = ChoosePixelFormat(renderWindowDC_, &pfd);
    SetPixelFormat(renderWindowDC_, pixelFormat, &pfd);

    // create and enable the render context (RC)
    hglrc_ = wglCreateContext(renderWindowDC_);
    wglMakeCurrent(renderWindowDC_, hglrc_);

    //ReleaseDC(hwnd_, hdc);
}

void ManglMainWindow::releaseOpenGLContext() {
    if (hglrc_) {
        // Release OpenGL context
        wglMakeCurrent(0, 0);
        wglDeleteContext(hglrc_);
        hglrc_ = 0;
    }
}


void ManglMainWindow::releaseOpenGL() {
    releaseOpenGLContext();
}

} // namespace mangl

#endif // if OpenGL
