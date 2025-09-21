/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#include "mangl_win_main_window.h"

#if MANGL_WINMAIN

namespace mangl {

DWORD ManglMainWindow::makeMainWindowStyleFlags() {
    DWORD windowStyle = WS_VISIBLE;

    windowStyle |= WS_CAPTION | WS_SYSMENU;
    windowStyle |= WS_CLIPCHILDREN;

#if MANGL_WIN_POPUP
    windowStyle |= WS_POPUP;
#endif

    windowStyle |= WS_CLIPCHILDREN;


    // Allowing resizing


    windowStyle |= WS_MAXIMIZEBOX;
    windowStyle |= WS_MINIMIZEBOX;
    windowStyle |= WS_SIZEBOX;

    if (stateWindowMaximized.get(false)) {
        isMaximized_ = true;
        windowStyle |= WS_MAXIMIZE;
    }

    return windowStyle;
}


RectI ManglMainWindow::makeMainWindowRect(DWORD windowStyle) {
    auto appRect = Env::applicationRect;

    if (M::isInvalidCord(appRect.w) || M::isInvalidCord(appRect.h))
        appRect.resize(Env::DefaultAppSize);

    currentDpi_ = GetDpiForSystem();

    auto appRectI = appRect.convert<RectI>();
    if (M::isInvalidCord(appRect.x))
        appRectI.x = M::InvalidCordI;
    if (M::isInvalidCord(appRect.y))
        appRectI.y = M::InvalidCordI;


    if (currentDpi_ > 96) {
        Cord coeff = Cord(currentDpi_) / Cord(96);

        appRectI.w = (CordI)(Cord(appRectI.w) * coeff);
        appRectI.h = (CordI)(Cord(appRectI.h) * coeff);
    }


    // Get the whole screen size
    auto screenSize = Env::physScreenSize.convert<SizeI>();

    // Get the target windows size
    auto wrect = winRectFromSizeI(appRectI.size());


    AdjustWindowRectEx(&wrect, windowStyle, false, 0);
    auto windowSizeI = makeSizeIFromRECT(wrect);
    auto windowSize = stateWindowSize.get(windowSizeI);

    // Check the ratio
    auto appRectRatio = Cord(windowSizeI.h) / Cord(windowSizeI.w);
    auto storedRatio = Cord(windowSize.h) / Cord(windowSize.w);
    if (std::abs(storedRatio - appRectRatio) > 0.0000001) {

        auto ws = windowSize.convert<Size>();

        ws.w = ws.h / appRectRatio;

        windowSize = ws.convert<SizeI>();
    }


    // Get safe desktop area
    SystemParametersInfo(SPI_GETWORKAREA, 0, &wrect, 0);
    auto desktopRect = makeRectIFromRECT(wrect);
    desktopRect.flip(screenSize.h);

    auto wndRect = desktopRect.centered(windowSize);

    if (M::isInvalidCordI(appRectI.x) || M::isInvalidCordI(appRectI.y)) {

        auto topLeft = stateWindowTopLeft.get(wndRect.position());

        RECT monRect{};
        monRect.left = topLeft.x;
        monRect.top = topLeft.y;
        monRect.right = topLeft.x + wndRect.w;
        monRect.bottom = topLeft.y + wndRect.h;

        auto hmon = MonitorFromRect(&monRect, MONITOR_DEFAULTTONULL);

        auto monitorRect = desktopRect;
        

        if (hmon) {
            MONITORINFO mi{};
            mi.cbSize = sizeof(mi);
            if (GetMonitorInfo(hmon, &mi)) {
                monitorRect = makeRectIFromRECT(mi.rcMonitor);
                screenSize.w = mi.rcMonitor.right - mi.rcMonitor.left;
                screenSize.h = std::abs(mi.rcMonitor.top - mi.rcMonitor.bottom);
            }
        } 

        wndRect.move({topLeft.x, screenSize.h - wndRect.h - topLeft.y});

        if (wndRect.right() > monitorRect.right())
            wndRect.x = monitorRect.right() - wndRect.w;
        if (wndRect.x < monitorRect.left())
            wndRect.x = monitorRect.left();

        if (wndRect.y < monitorRect.y)
            wndRect.y = monitorRect.y;

        if (wndRect.top() > monitorRect.top())
            wndRect.y = monitorRect.top() - wndRect.h;

    } else {
        wndRect.move({(int)appRectI.x, (int)appRectI.y});
    }

    appRectI.x = wndRect.x;
    appRectI.y = wndRect.y;
    // Env::applicationRect = appRectI.convert<Rect>();

    // Final flip before sending to windows API
    wndRect.flip(screenSize.h);
    if (wndRect.y < 0)
        wndRect.y = 0;

    return wndRect;
}

Rect ManglMainWindow::computeRenderWindowLayout() {
    RECT wClientRect{};
    GetClientRect(hwnd_, &wClientRect);

    auto clientRect = makeRectFromRECT(wClientRect);

    return computeRenderWindowLayout(clientRect);
}

Rect ManglMainWindow::computeRenderWindowLayout(RectA clientRect) {

    Cord layoutRatio = 1;

    if (Env::layoutSize.h != 0 && Env::layoutSize.w != 0)
        layoutRatio = Env::layoutSize.h / Env::layoutSize.w;
    else
        layoutRatio = Env::applicationRect.h / Env::applicationRect.w;


    Rect renderRect{};

    bool fitHeight = true;

    if (clientRect.h / layoutRatio > clientRect.w)
        fitHeight = false;

    if (fitHeight) {
        renderRect.h = clientRect.h;
        renderRect.w = clientRect.h / layoutRatio;
    } else {
        renderRect.w = clientRect.w;
        renderRect.h = clientRect.w * layoutRatio;
    }

    renderRect.x = (clientRect.w - renderRect.w) / 2;
    renderRect.y = (clientRect.h - renderRect.h) / 2;


    return renderRect;
}


void ManglMainWindow::processRenderLayoutChange(RectA layout) {
    Env::applicationRect = layout;

    Env::processScreenLayout();
    Env::screenScale = Env::applicationRect.w / Env::layoutSize.w;
    Env::layoutPadding = Env::screenPadding / Env::screenScale;


    Env::updateConversionData();
}


void ManglMainWindow::processDpiChange() {
    // auto newDpi = Real(xDpi);

    if (dpiChangeX_ == currentDpi_)
        return;

    auto newPixelDensity = Cord(dpiChangeX_) / M::winDefaultDpi;
    auto rect = makeRectIFromRECT(dpiSuggestedWinRect_);

    currentDpi_ = dpiChangeX_;

    ::SetWindowPos(hwnd_, 0, rect.x, rect.y, rect.w, rect.h, SWP_NOZORDER | SWP_NOACTIVATE);

    RECT clientRect{};
    ::GetClientRect(hwnd_, &clientRect);

    processRenderLayoutChange(makeRectFromRECT(clientRect));


    // mainLoopPendingLayout_ = true;
}

void ManglMainWindow::setFullScreen(bool fullscreen) {
    if (isFullScreen_ == fullscreen)
        return;

    // Save current window statue

    if (!isFullScreen_) {
        fullscreenMaximized_ = !!::IsZoomed(hwnd_);

        if (fullscreenMaximized_)
            ::SendMessageW(hwnd_, WM_SYSCOMMAND, SC_RESTORE, 0);

        fullscreenSavedStyle_ = ::GetWindowLongW(hwnd_, GWL_STYLE);
        fullscreenSavedExStyle_ = ::GetWindowLongW(hwnd_, GWL_EXSTYLE);

        ::GetWindowRect(hwnd_, &fullscreenSavedWindowRect_);
    }

    isFullScreen_ = fullscreen;

    if (isFullScreen_) {
        ::SetWindowLong(hwnd_, GWL_STYLE, fullscreenSavedStyle_ & ~(WS_CAPTION | WS_THICKFRAME));
        ::SetWindowLong(
            hwnd_, GWL_EXSTYLE,
            fullscreenSavedExStyle_ &
                ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE)
        );

        MONITORINFO mi{};
        mi.cbSize = sizeof(mi);
        ::GetMonitorInfoW(MonitorFromWindow(hwnd_, MONITOR_DEFAULTTONEAREST), &mi);

        auto rect = makeRectIFromRECT(mi.rcMonitor);
        ::SetWindowPos(
            hwnd_, NULL, rect.x, rect.y, rect.w, rect.h,
            SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED
        );
    } else {

        ::SetWindowLong(hwnd_, GWL_STYLE, fullscreenSavedStyle_);
        ::SetWindowLong(hwnd_, GWL_EXSTYLE, fullscreenSavedExStyle_);

        auto rect = makeRectIFromRECT(fullscreenSavedWindowRect_);

        ::SetWindowPos(
            hwnd_, NULL, rect.x, rect.y, rect.w, rect.h,
            SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED
        );

        if (fullscreenMaximized_)
            ::SendMessage(hwnd_, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    }
}


} // namespace mangl

#endif
