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

#include <uxtheme.h>
#pragma comment(lib, "UxTheme.lib")

#pragma comment(lib, "Comctl32.lib")



namespace mangl {

static LRESULT CALLBACK mainWndProcFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return ManglMainWindow::instance()->mainWndProc(hWnd, message, wParam, lParam);
}

static LRESULT CALLBACK renderWndProcFunc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    return ManglMainWindow::instance()->renderWndProc(hWnd, message, wParam, lParam);
}



ManglMainWindow::ManglMainWindow() {
    //fps_ = 30;
    fps_ = 60;

#ifdef MANGL_FPS
    fps_ = MANGL_FPS;
#endif
}

ManglMainWindow::~ManglMainWindow() {}


void ManglMainWindow::createMainWindow(HWND parentId, RectI rect, int flags) {
    auto msgFlags = MB_OK | MB_ICONEXCLAMATION;

    timestamp_ = 0;
    mouseRightDown_ = mouseLeftDown_ = false;


    InitCommonControls();

    auto dpi = GetDpiForSystem();


    HICON windowIcon{}, windowIconSm{};

    if (!Env::windowIcon.empty()) {
        auto resIndex = apl::parseInt(Env::windowIcon.substr(1));
        auto iconRes = MAKEINTRESOURCEW(resIndex);
        // windowIcon = (HICON)LoadImageW(hInstance_, iconRes, IMAGE_ICON,
        // GetSystemMetrics(SM_CXICON),
        //                                GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
        windowIcon = (HICON)LoadImageW(
            hInstance_, iconRes, IMAGE_ICON, GetSystemMetricsForDpi(SM_CXICON, dpi),
            GetSystemMetricsForDpi(SM_CYICON, dpi), LR_DEFAULTCOLOR
        );
        windowIconSm = (HICON)LoadImageW(
            hInstance_, iconRes, IMAGE_ICON, GetSystemMetricsForDpi(SM_CXSMICON, dpi),
            GetSystemMetricsForDpi(SM_CYSMICON, dpi), LR_DEFAULTCOLOR
        );
    } else {
        // IDI_APPLICATION - 32512
        windowIcon = LoadIconW(NULL, MAKEINTRESOURCEW(32512));
        windowIconSm = LoadIconW(NULL, MAKEINTRESOURCEW(32512));
    }

    // Windows variables
    auto mainWndClassName = L"Mangl::MainWindow";
    auto renderWndClassName = L"Mangl::RenderWindow";

    if (!mainWndClass_) {

        auto backgroundBrush = CreateSolidBrush(RGB(30, 30, 30));

        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(wc);
        wc.lpfnWndProc = mainWndProcFunc;
        wc.hInstance = hInstance_;
        //wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
        wc.hbrBackground = backgroundBrush;
        wc.lpszClassName = mainWndClassName;
        wc.hIcon = windowIcon;
        wc.hIconSm = windowIconSm;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);

        mainWndClass_ = RegisterClassExW(&wc);
        if (!mainWndClass_) {
            MessageBoxW(0, L"Failed to register the window class", L"Error", msgFlags);
            return;
        }
    }

    if (!renderWndClass_) {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = renderWndProcFunc;
        wc.hInstance = hInstance_;
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wc.lpszClassName = renderWndClassName;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);

        renderWndClass_ = RegisterClassEx(&wc);
        if (!renderWndClass_) {
            MessageBoxW(0, L"Failed to register the window class", L"Error", msgFlags);
            return;
        }
    }


    // auto screenSize = Env::instance()->screenSize;
    apl::ApiString wndTitle;
    
    if (!Env::windowTitle.empty()) {
        wndTitle = Env::windowTitle;
    } else {
#ifdef MANGL_APP_TITLE
        wndTitle = MANGL_APP_TITLE;
#else
        wndTitle = Env::appName;
#endif
    }

    hwnd_ = CreateWindowExW(
        0, mainWndClassName, wndTitle.win(), flags, rect.x, rect.y, rect.w, rect.h, parentId, 0,
        hInstance_, 0
    );

    if (!hwnd_) {
        MessageBoxW(0, L"Failed to create main window", L"Error", msgFlags);
        return;
    }


    htheme_ = OpenThemeData(hwnd_, L"WINDOW");
    sysVisualStylesEnabled_ = htheme_ != 0;

    if (sysVisualStylesEnabled_) {
        // TMT_BACKGROUND  = 1602
        //auto brush = GetThemeSysColorBrush(htheme_, 1602);
        //SetClassLongPtrW(hwnd_, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    }


    if (windowIcon)
        SetClassLongPtr(hwnd_, GCLP_HICON, (LONG_PTR)windowIcon);

    if (windowIconSm)
        SetClassLongPtr(hwnd_, GCLP_HICONSM, (LONG_PTR)windowIconSm);


    int style = WS_VISIBLE | WS_CHILD;
    auto renderRect = computeRenderWindowLayout();

    renderHwnd_ = CreateWindowEx(
        0, renderWndClassName, 0, style, 
        (int)renderRect.x, (int)renderRect.y, (int)renderRect.w, (int)renderRect.h, 
        hwnd_, 0,
        hInstance_, 0
    );

    if (!renderHwnd_) {
        MessageBoxW(0, L"Failed to create main window", L"Error", msgFlags);
        return;
    }



    gameLoopInit();
    gameLoopStart();
}


void ManglMainWindow::destroyCleanup() { gameLoopStop(); }

void ManglMainWindow::destroyMainWindow() {
    if (hwnd_)
        DestroyWindow(hwnd_);
    hwnd_ = 0;
}

} // namespace mangl

#endif
