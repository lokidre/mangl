/**************************************************************************
 *   MANGL - Mobile Application and Game Libray                           *
 *   Copyright (C) 2009-2025 Andrei Mirzoyan                              * 
 *                                                                        *
 *   This Source Code Form is subject to the terms of the Mozilla Public  *
 *   License, v. 2.0. If a copy of the MPL was not distributed with this  *
 *   file, You can obtain one at http://mozilla.org/MPL/2.0/.             *
 **************************************************************************/
#pragma once

#include "../core/mangl_setup.h"
#include "../core/mangl_framework.h"
#include "../core/mangl_storage.h"

#if MANGL_WINMAIN

#include "mangl_win_util.h"

#include <apeal/event.hpp>

#define MANGL_WM_PROCESS_FRAMEWORK_ACTIONS WM_APP + 1

#include <timeapi.h>


namespace mangl {

class ManglMainWindow: public apl::Singleton<ManglMainWindow, false> {
private:
    Setting<PointI> stateWindowTopLeft{"mangl.win.main_window.top_left"};
    Setting<SizeI> stateWindowSize{"mangl.win.main_window.size"};
    Setting<bool> stateWindowMaximized{"mangl.win.main_window.maximized"};
    Setting<bool> stateWindowFullScreen{"mangl.win.main_window.fullscreen"};

public:
    Timestamp fps_{};

public:
    ManglMainWindow();
    ~ManglMainWindow();

    void createMainWindow(HWND parentId, RectI rect, int flags);
    void destroyMainWindow();
    void destroyCleanup();

    HWND getSystemHandle() { return hwnd_; }

private:
    DWORD makeMainWindowStyleFlags();
    RectI makeMainWindowRect(DWORD styles);
    void processDpiChange();
    void processRenderLayoutChange(RectA layout);
    Rect computeRenderWindowLayout();
    Rect computeRenderWindowLayout(RectA clientRect);


    // Launching
    bool loopJustLaunched_{true};

    // Maximize support
    bool isMaximized_{};

    // Fullscreen support
    bool isFullScreen_{};
    bool fullscreenMaximized_{};
    LONG fullscreenSavedStyle_{}, fullscreenSavedExStyle_{};
    RECT fullscreenSavedWindowRect_{};
    void setFullScreen(bool fullscreen);
    void toggleFullScreen() { setFullScreen(!isFullScreen_); }


    // Keyboard shortcuts support
    std::vector<KeyCode> currentPressedKeys_;
    void removePressedKey(KeyCode code) {
        //currentPressedKeys_.erase(std::remove_if(currentPressedKeys_.begin(), currentPressedKeys_.end(), code));

        //std::erase_if(currentPressedKeys_, code);
        std::erase(currentPressedKeys_, code);
    }


public:
    int onWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

    HWND hwnd() const noexcept { return hwnd_; }
    HINSTANCE hinstance() const noexcept { return hInstance_; }

//#if MANGL_WIN_GAME_LOOP_TIMER_MM
    void multimediaTimerCallback();
    void mmCallbackError(const char* err);
//#endif

    void processFrameworkActions(HDC hdc);
    bool processFrameworkActionsGui(HDC hdc, const MFrameworkAction& action);

    LRESULT mainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT renderWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void handleWndProcException(StrView message);

    bool exitOnClose{true};


private:
    HINSTANCE hInstance_{}, hPrevInstance_{};
    bool sysVisualStylesEnabled_{};
    HWND hwnd_{}, renderHwnd_{};
    HTHEME htheme_{};
    HDC renderWindowDC_{};
    ATOM mainWndClass_{}, renderWndClass_{};

    int currentDpi_{};
    bool dpiChangePending_{};
    RECT dpiSuggestedWinRect_{};
    int dpiChangeX_{}, dpiChangeY_{};


    Timestamp timestamp_{};
    bool mouseLeftDown_{}, mouseRightDown_{};
    bool mouseTracking_{};


    //
    // Game loop variables
    //
    bool mainLoopPendingInit_{true};
    bool mainLoopPendingLayout_{false};
    bool mainLoopPendingExit_{false};

#if MANGL_WIN_GAME_LOOP_TIMER_MM
    MMRESULT timerHandle_{};
    apl::AEvent timerExitEvent_{};
    HANDLE timerQueueHandle_{};
#endif

#if MANGL_OPENGL
    HGLRC hglrc_{};

    void initOpenGL();
    void createOpenGLContext();
    PIXELFORMATDESCRIPTOR initOpenGLPixelDescriptor();
    void releaseOpenGLContext();
    void releaseOpenGL();
#endif


    void gameLoopInit();
    void gameLoopStart();
    void gameLoopStop();


// Audio MCI support
#if MANGL_MUSIC_MCI
    friend class MusicEngineMCI;
    MusicEngineMCI* mciDevice_{};
#endif


public:
    struct WinGuiWidget {
        enum class Type { Invalid = 0, Label, Input };
        Type type{};
        WidgetId id{};
        HWND hwnd{};
        HBRUSH hbrush{};
        HFONT hfont{};
        Color textColor{}, bkCol{};
        Rect contentsFrame{};
        Flags flags{};
        String text{};
        bool remove{};

        void invalidate() { InvalidateRect(hwnd, 0, FALSE); }
    };

    using WinGuiWidgetList = KeyValCont<WidgetId, WinGuiWidget*>;

    WinGuiWidgetList winGuiWidgets_;
    WidgetId winGuiWidgetId_{1};


    WinGuiWidget* widgetById(WidgetId id) {
        if (auto it = winGuiWidgets_.find(id); it != winGuiWidgets_.end())
            return it->second;
        // throw std::runtime_error("Widget not found: " + std::to_string(controlId)) ;
        return 0;
    }

    WinGuiWidget* newWidget(WinGuiWidget::Type type) {
        // TODO: replace with unique_ptr
        auto w = new WinGuiWidget;
        winGuiWidgets_[winGuiWidgetId_] = w;
        w->id = winGuiWidgetId_++;
        w->type = type;
        return w;
    }

    auto _MRectToWindowsRect(Rect rect) noexcept {
        auto k = Env::screenScale;
        rect.scale(k);

        return RectI::make(rect.x, Env::applicationRect.h - 1 - rect.y - rect.h, rect.w, rect.h);
    }

    auto getTextBoundary(HDC hdc, StrView text, RectA rect) noexcept {
        auto wrect = _MRectToWindowsRect(rect);
        apl::ApiString apiText{text};
        SIZE textBoundary{};
        if (::GetTextExtentPoint32W(hdc, apiText.win(), apiText.winLength(), &textBoundary)) {
            wrect.y += (wrect.h - textBoundary.cy) / 2;
            wrect.h = textBoundary.cy;
            wrect.w = textBoundary.cx;
        }
        return wrect;
    }

    void updateWidgetRect(HDC hdc, WinGuiWidget* widget) {
        auto wrect = _MRectToWindowsRect(widget->contentsFrame);

        if (widget->type == WinGuiWidget::Type::Label) {
            apl::ApiString apiText{widget->text};
            auto oldFont = (HFONT)SelectObject(hdc, widget->hfont);
            SIZE textBoundary;
            if (::GetTextExtentPoint32W(hdc, apiText.win(), apiText.winLength(), &textBoundary)) {
                if (widget->flags & M::AlignCenter)
                    wrect.x += (wrect.w - textBoundary.cx) / 2;
                if (widget->flags & M::AlignVCenter)
                    wrect.y += (wrect.h - textBoundary.cy) / 2;

                wrect.h = textBoundary.cy;
                wrect.w = textBoundary.cx;
            }
            ::SelectObject(hdc, oldFont);
        } else {
        }

        ::MoveWindow(widget->hwnd, wrect.x, wrect.y, wrect.w, wrect.h, TRUE);
    }
};


} // namespace mangl


#endif

